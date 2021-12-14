#pragma once
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include<mfidl.h>
#include<mfreadwrite.h>;
#include "image/codecs/codec.h";
#include <audio\timestamp.h>



namespace Image {

	class WMFStream : public IMFByteStream {
	public:
		WMFStream(Common::SeekableReadStream* Stream);
		virtual ~WMFStream();
	//IUnknown
		HRESULT STDMETHODCALLTYPE QueryInterface(
			/* [in] */ REFIID riid,
			/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) override;

		ULONG STDMETHODCALLTYPE AddRef(void) override;

		ULONG STDMETHODCALLTYPE Release(void) override;
	//IMFByteStream
		HRESULT STDMETHODCALLTYPE GetCapabilities(
			/* [out] */ __RPC__out DWORD *pdwCapabilities) override;

		HRESULT STDMETHODCALLTYPE GetLength(
			/* [out] */ __RPC__out QWORD *pqwLength) override;

		HRESULT STDMETHODCALLTYPE SetLength(
			/* [in] */ QWORD qwLength) override;

		HRESULT STDMETHODCALLTYPE GetCurrentPosition(
			/* [out] */ __RPC__out QWORD *pqwPosition) override;

		HRESULT STDMETHODCALLTYPE SetCurrentPosition(
			/* [in] */ QWORD qwPosition) override;

		HRESULT STDMETHODCALLTYPE IsEndOfStream(
			/* [out] */ __RPC__out BOOL *pfEndOfStream) override;

		HRESULT STDMETHODCALLTYPE Read(
			/* [size_is][out] */ __RPC__out_ecount_full(cb) BYTE *pb,
			/* [in] */ ULONG cb,
			/* [out] */ __RPC__out ULONG *pcbRead) override;

		 /* [local] */ HRESULT STDMETHODCALLTYPE BeginRead(
			/* [annotation][out] */
			_Out_writes_bytes_(cb) BYTE *pb,
			/* [in] */ ULONG cb,
			/* [in] */ IMFAsyncCallback *pCallback,
			/* [in] */ IUnknown *punkState) override;

		 /* [local] */ HRESULT STDMETHODCALLTYPE EndRead(
			/* [in] */ IMFAsyncResult *pResult,
			/* [annotation][out] */
			_Out_ ULONG *pcbRead) override;

		 HRESULT STDMETHODCALLTYPE Write(
			/* [size_is][in] */ __RPC__in_ecount_full(cb) const BYTE *pb,
			/* [in] */ ULONG cb,
			/* [out] */ __RPC__out ULONG *pcbWritten);

		 /* [local] */ HRESULT STDMETHODCALLTYPE BeginWrite(
			/* [annotation][in] */
			_In_reads_bytes_(cb) const BYTE *pb,
			/* [in] */ ULONG cb,
			/* [in] */ IMFAsyncCallback *pCallback,
			/* [in] */ IUnknown *punkState) override;

		 /* [local] */ HRESULT STDMETHODCALLTYPE EndWrite(
			/* [in] */ IMFAsyncResult *pResult,
			/* [annotation][out] */
			_Out_ ULONG *pcbWritten) override;

		 HRESULT STDMETHODCALLTYPE Seek(
			/* [in] */ MFBYTESTREAM_SEEK_ORIGIN SeekOrigin,
			/* [in] */ LONGLONG llSeekOffset,
			/* [in] */ DWORD dwSeekFlags,
			/* [out] */ __RPC__out QWORD *pqwCurrentPosition) override;

		 HRESULT STDMETHODCALLTYPE Flush(void) override;

		HRESULT STDMETHODCALLTYPE Close(void) override;

	private:
		Common::SeekableReadStream* _Stream;
		int64 _lastAsyncRead = 0;
	};

	class WMFDecoder : public Codec {

	public:
		WMFDecoder(int Width, int Height);
		~WMFDecoder();

		const Graphics::Surface *decodeFrame(Common::SeekableReadStream &stream);

		Graphics::PixelFormat getPixelFormat() const { return Graphics::PixelFormat(4, 8, 8, 8, 0, 16, 8, 0, 0); };

		bool IsHighRes() override { return true; }
		void Seek(Audio::Timestamp time) override;
	private:
		IMFSourceReader *_reader;
		IMFByteStream *_byteStream;
		Common::SeekableReadStream *_usedStream;
		Graphics::Surface* _target;
		int _width;
		int _height;
	};
}
