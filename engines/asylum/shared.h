/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

#ifndef ASYLUM_SHARED_H
#define ASYLUM_SHARED_H_

#include "common/singleton.h"

#include "asylum/sceneres.h"

namespace Asylum {

/**
 * Shared Resources are classes that are instantiated at the engine level.
 * Binding them to this singleton simplifies implementation of new classes
 * that will require access to common interfaces and tools.
 *
 * A candidate for a shared resource would be any class that will be utilized
 * by one-to-many instances of another class, but will never need more than
 * one instance itself.
 *
 * Each component herin could technically be a singleton unto itself, but
 * by using this method, destruction can be handled by the engine.
 */
class SharedResources: public Common::Singleton<SharedResources> {
public:

    void setOSystem(OSystem* system) { _system = system; }
    uint32 getMillis() { return _system->getMillis(); }

	void setVideo(Video* video) { _video = video; }
	Video* getVideo() { return _video; }

	void setScreen(Screen* screen) { _screen = screen; }
	Screen* getScreen() { return _screen; }

	void setSound(Sound* sound) { _sound = sound; }
	Sound* getSound() { return _sound; }

	void setScene(Scene* scene) { _scene = scene; }
	Scene* getScene() { return _scene; }

	int getAngle(int x1, int y1, int x2, int y2);

    void setGameFlag(int flag);
    void clearGameFlag(int flag);
    void toggleGameFlag(int flag);
    bool isGameFlagSet(int flag);
    bool isGameFlagNotSet(int flag);

private:
	friend class Common::Singleton<SingletonBaseType>;
	SharedResources();
	~SharedResources();

    OSystem *_system;
	Video   *_video;
	Screen  *_screen;
	Sound   *_sound;
	Scene   *_scene;

	int _gameFlags[1512];

}; // end of class SharedResources

	// Angle Tables used by getAngle()
	const uint8 angleTable01[256] = {
		0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x02,0x02,0x02,0x02,0x02,0x03,0x03,0x03,0x03,
		0x04,0x04,0x04,0x04,0x04,0x05,0x05,0x05,0x05,0x06,0x06,0x06,0x06,0x06,0x07,0x07,
		0x07,0x07,0x08,0x08,0x08,0x08,0x08,0x09,0x09,0x09,0x09,0x0A,0x0A,0x0A,0x0A,0x0A,
		0x0B,0x0B,0x0B,0x0B,0x0B,0x0C,0x0C,0x0C,0x0C,0x0D,0x0D,0x0D,0x0D,0x0D,0x0E,0x0E,
		0x0E,0x0E,0x0E,0x0F,0x0F,0x0F,0x0F,0x10,0x10,0x10,0x10,0x10,0x11,0x11,0x11,0x11,
		0x11,0x12,0x12,0x12,0x12,0x12,0x13,0x13,0x13,0x13,0x13,0x14,0x14,0x14,0x14,0x14,
		0x15,0x15,0x15,0x15,0x15,0x16,0x16,0x16,0x16,0x16,0x16,0x17,0x17,0x17,0x17,0x17,
		0x18,0x18,0x18,0x18,0x18,0x19,0x19,0x19,0x19,0x19,0x19,0x1A,0x1A,0x1A,0x1A,0x1A,
		0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1C,0x1C,0x1C,0x1C,0x1C,0x1D,0x1D,0x1D,0x1D,0x1D,
		0x1D,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x20,0x20,0x20,
		0x20,0x20,0x20,0x20,0x21,0x21,0x21,0x21,0x21,0x21,0x22,0x22,0x22,0x22,0x22,0x22,
		0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x25,0x25,
		0x25,0x25,0x25,0x25,0x25,0x26,0x26,0x26,0x26,0x26,0x26,0x26,0x27,0x27,0x27,0x27,
		0x27,0x27,0x27,0x27,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x29,0x29,0x29,0x29,0x29,
		0x29,0x29,0x29,0x2A,0x2A,0x2A,0x2A,0x2A,0x2A,0x2A,0x2A,0x2B,0x2B,0x2B,0x2B,0x2B,
		0x2B,0x2B,0x2B,0x2C,0x2C,0x2C,0x2C,0x2C,0x2C,0x2C,0x2C,0x2C,0x2D,0x2D,0x2D,0x2D
	};

	const uint8 angleTable02[256] = {
		0x00,0x04,0x07,0x0B,0x0E,0x11,0x15,0x18,0x1B,0x1D,0x20,0x23,0x25,0x27,0x29,0x2B,
		0x2D,0x2F,0x30,0x32,0x33,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
		0x3F,0x40,0x41,0x41,0x42,0x43,0x43,0x44,0x44,0x45,0x45,0x46,0x46,0x46,0x47,0x47,
		0x48,0x48,0x48,0x49,0x49,0x49,0x49,0x4A,0x4A,0x4A,0x4B,0x4B,0x4B,0x4B,0x4C,0x4C,
		0x4C,0x4C,0x4C,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4F,
		0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x50,0x50,0x50,0x50,0x50,0x50,0x50,0x50,0x50,
		0x51,0x51,0x51,0x51,0x51,0x51,0x51,0x51,0x51,0x51,0x51,0x51,0x52,0x52,0x52,0x52,
		0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x52,0x53,0x53,0x53,0x53,0x53,0x53,
		0x53,0x53,0x53,0x53,0x53,0x53,0x53,0x53,0x53,0x53,0x53,0x53,0x53,0x54,0x54,0x54,
		0x54,0x54,0x54,0x54,0x54,0x54,0x54,0x54,0x54,0x54,0x54,0x54,0x54,0x54,0x54,0x54,
		0x54,0x54,0x54,0x54,0x54,0x54,0x54,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
		0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x56,0x56,0x56,0x56,
		0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,
		0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,
		0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56
	};

	const uint8 angleTable03[256] = {
		0x00,0x45,0x63,0x72,0x76,0x79,0x81,0x82,0x83,0x84,0x84,0x85,0x85,0x86,0x86,0x86,
		0x86,0x87,0x87,0x87,0x87,0x87,0x87,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,
		0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,
		0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,
		0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,
		0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,
		0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,0x89,
		0x89,0x89,0x89,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,
		0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,
		0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,
		0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,
		0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,
		0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,
		0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,
		0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,
		0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90
	};

#define Shared	(::Asylum::SharedResources::instance())

} // end of namespace Asylum

#endif