#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include "WMF.h"
#include "common/textconsole.h"
#include "common/stream.h"
#include<objbase.h>
#include<mfapi.h>
#include<mfidl.h>
#include<mfreadwrite.h>
#include<Mferror.h>


void Check(const HRESULT AResult) {
	if (AResult != S_OK)
		error("Failed with HResult: 0x%08x", AResult);
}

void StartMF() {
	CoInitialize(nullptr);
	Check(MFStartup(MF_VERSION));
}

void StopMF() {
	Check(MFShutdown());
	CoUninitialize();
}

template<class T>
void SafeRelease(T **ppT) {
	if (*ppT) {
		(*ppT)->Release();
		*ppT = NULL;
	}
}

HRESULT ConfigureDecoder(IMFSourceReader *pReader, DWORD dwStreamIndex) {
	IMFMediaType *pNativeType = NULL;
	IMFMediaType *pType = NULL;

	// Find the native format of the stream.
	HRESULT hr = pReader->GetNativeMediaType(dwStreamIndex, 0, &pNativeType);
	if (FAILED(hr)) {
		return hr;
	}

	GUID majorType, subtype;

	// Find the major type.
	hr = pNativeType->GetGUID(MF_MT_MAJOR_TYPE, &majorType);
	if (FAILED(hr)) {
		goto done;
	}

	// Define the output type.
	hr = MFCreateMediaType(&pType);
	if (FAILED(hr)) {
		goto done;
	}

	hr = pType->SetGUID(MF_MT_MAJOR_TYPE, majorType);
	if (FAILED(hr)) {
		goto done;
	}

	// Select a subtype.
	if (majorType == MFMediaType_Video) {
		subtype = MFVideoFormat_RGB32;
	} else if (majorType == MFMediaType_Audio) {
		subtype = MFAudioFormat_PCM;
	} else {
		// Unrecognized type. Skip.
		goto done;
	}

	hr = pType->SetGUID(MF_MT_SUBTYPE, subtype);
	if (FAILED(hr)) {
		goto done;
	}

	// Set the uncompressed format.
	hr = pReader->SetCurrentMediaType(dwStreamIndex, NULL, pType);
	if (FAILED(hr)) {
		goto done;
	}

done:
	SafeRelease(&pNativeType);
	SafeRelease(&pType);
	return hr;
}

HRESULT EnumerateTypesForStream(IMFSourceReader *pReader, DWORD dwStreamIndex) {
	HRESULT hr = S_OK;
	DWORD dwMediaTypeIndex = 0;

	while (SUCCEEDED(hr)) {
		IMFMediaType *pType = NULL;
		hr = pReader->GetNativeMediaType(dwStreamIndex, dwMediaTypeIndex, &pType);
		if (hr == MF_E_NO_MORE_TYPES) {
			hr = S_OK;
			break;
		} else if (SUCCEEDED(hr)) {
			// Examine the media type. (Not shown.)
			GUID SubType;
			pType->GetGUID(MF_MT_SUBTYPE, &SubType);
			pType->Release();
		}
		++dwMediaTypeIndex;
	}
	return hr;
}

Image::WMFDecoder::WMFDecoder(int Width, int Height) : Codec(), _width(Width), _height(Height), _reader(nullptr), _target(nullptr), _byteStream(nullptr), _usedStream(nullptr) {
	StartMF();
}

Image::WMFDecoder::~WMFDecoder() {
	if (_target) {
		_target->free();
		delete _target;
	}
	if (_reader)
		SafeRelease(&_reader);
	StopMF();
	//we do not implement ref counting so just call it a day here ;)
	delete _byteStream;
}

const Graphics::Surface *Image::WMFDecoder::decodeFrame(Common::SeekableReadStream &stream) {
	if ((!_reader) || (_usedStream != &stream)) {
		SafeRelease(&_reader);
		if (_byteStream)
			delete _byteStream;
		IMFAttributes *Attributes = nullptr;
		Check(MFCreateAttributes(&Attributes, 2));
		Attributes->SetUINT32(MF_SOURCE_READER_ENABLE_ADVANCED_VIDEO_PROCESSING, 1);
		Attributes->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_IYUV);
		_byteStream = new WMFStream(&stream);
		_usedStream = &stream;
		Check(MFCreateSourceReaderFromByteStream(_byteStream, Attributes, &_reader));
		Check(ConfigureDecoder(_reader, MF_SOURCE_READER_FIRST_VIDEO_STREAM));
		SafeRelease(&Attributes);
	}
	//EnumerateTypesForStream(_reader, MF_SOURCE_READER_FIRST_VIDEO_STREAM);
	if (!_target) {
		_target = new Graphics::Surface();
		_target->create(_width, _height, this->getPixelFormat());
	}
	DWORD streamindex, flags;
	LONGLONG llTimeStamp;
	IMFSample *sample = nullptr;
	Check(_reader->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &streamindex, &flags, &llTimeStamp, &sample));
	if (sample) {
		IMFMediaBuffer *buffer = nullptr;
		//Check(sample->GetBufferByIndex(0, &buffer));
		sample->ConvertToContiguousBuffer(&buffer);
		BYTE *Pixels = nullptr;
		DWORD MaxLength, CurrentLength;
		Check(buffer->Lock(&Pixels, &MaxLength, &CurrentLength));
		int Expected = _width * _height * 4;
		if (Expected == CurrentLength)
			memcpy(_target->getPixels(), Pixels, CurrentLength);
		Check(buffer->Unlock());
		SafeRelease(&buffer);
		SafeRelease(&sample);
	}
	return _target;
}

Image::WMFStream::WMFStream(Common::SeekableReadStream* Stream): IMFByteStream(), _Stream(Stream) {
}

Image::WMFStream::~WMFStream() {
}

HRESULT __stdcall Image::WMFStream::QueryInterface(REFIID riid, void **ppvObject) {
	if (riid == IID_IMFByteStream) {
		IMFByteStream *Result = this;
		*ppvObject = Result;
		return S_OK;
	}
	return E_NOINTERFACE;
}

ULONG __stdcall Image::WMFStream::AddRef(void) {
	return 0;
}

ULONG __stdcall Image::WMFStream::Release(void) {
	return 0;
}

HRESULT __stdcall Image::WMFStream::GetCapabilities(DWORD *pdwCapabilities) {
	*pdwCapabilities = MFBYTESTREAM_IS_READABLE | MFBYTESTREAM_IS_SEEKABLE;
	return S_OK;
}

HRESULT __stdcall Image::WMFStream::GetLength(QWORD *pqwLength) {
	*pqwLength = _Stream->size();
	return S_OK;
}

HRESULT __stdcall Image::WMFStream::SetLength(QWORD qwLength) {
	return E_NOTIMPL;
}

HRESULT __stdcall Image::WMFStream::GetCurrentPosition(QWORD *pqwPosition) {
	*pqwPosition = _Stream->pos();
	return S_OK;
}

HRESULT __stdcall Image::WMFStream::SetCurrentPosition(QWORD qwPosition) {
	_Stream->seek(qwPosition, SEEK_SET);
	return S_OK;
}

HRESULT __stdcall Image::WMFStream::IsEndOfStream(BOOL *pfEndOfStream) {
	*pfEndOfStream = _Stream->eos();
	return S_OK;
}

HRESULT __stdcall Image::WMFStream::Read(BYTE *pb, ULONG cb, ULONG *pcbRead) {
	*pcbRead = _Stream->read(pb, cb);
	return S_OK;
}

HRESULT __stdcall Image::WMFStream::BeginRead(BYTE *pb, ULONG cb, IMFAsyncCallback *pCallback, IUnknown *punkState) {
	_lastAsyncRead = _Stream->read(pb, cb);
	IMFAsyncResult *Result = nullptr;
	Check(MFCreateAsyncResult(nullptr, pCallback, punkState, &Result));
	Result->SetStatus(S_OK);
	pCallback->Invoke(Result);
	return S_OK;
}

HRESULT __stdcall Image::WMFStream::EndRead(IMFAsyncResult *pResult, ULONG *pcbRead) {
	*pcbRead = _lastAsyncRead;
	SafeRelease(&pResult);
	return S_OK;
}

HRESULT __stdcall Image::WMFStream::Write(const BYTE *pb, ULONG cb, ULONG *pcbWritten) {
	return E_NOTIMPL;
}

HRESULT __stdcall Image::WMFStream::BeginWrite(const BYTE *pb, ULONG cb, IMFAsyncCallback *pCallback, IUnknown *punkState) {
	return E_NOTIMPL;
}

HRESULT __stdcall Image::WMFStream::EndWrite(IMFAsyncResult *pResult, ULONG *pcbWritten) {
	return E_NOTIMPL;
}

HRESULT __stdcall Image::WMFStream::Seek(MFBYTESTREAM_SEEK_ORIGIN SeekOrigin, LONGLONG llSeekOffset, DWORD dwSeekFlags, QWORD *pqwCurrentPosition) {
	int Origin = SEEK_SET;
	if (SeekOrigin == msoCurrent)
		Origin = SEEK_CUR;
	_Stream->seek(llSeekOffset, Origin);
	*pqwCurrentPosition = _Stream->pos();
	return S_OK;
}

HRESULT __stdcall Image::WMFStream::Flush(void) {
	return S_OK;
}

HRESULT __stdcall Image::WMFStream::Close(void) {
	return S_OK;
}
