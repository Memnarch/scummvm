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

#ifndef ASYLUM_ENGINE_H
#define ASYLUM_ENGINE_H

#include "engines/engine.h"

#include "asylum/resman.h"
#include "asylum/scene.h"
#include "asylum/state.h"
#include "asylum/screen.h"
#include "asylum/sound.h"
#include "asylum/video.h"

namespace Asylum {

class Scene;
class ResourceManager;
class Screen;
class Menu;
class State;
class Sound;
class Video;

class AsylumEngine: public Engine {
public:

    AsylumEngine(OSystem *system, Common::Language language);
    virtual ~AsylumEngine();

    // Engine APIs
    Common::Error init();
    Common::Error go();
    virtual Common::Error run();
    virtual bool hasFeature(EngineFeature f) const;

	void checkForEvent(bool doUpdate);
	void waitForTimer(int msec_delay);
	void updateMouseCursor();

	Screen* getScreen() { return _screen; }
	ResourceManager* getResourceManager() { return _resMgr; }
	Sound* getSound() { return _sound; }

private:
    Common::Language     _language;
    Common::RandomSource _rnd;

    ResourceManager   *_resMgr;

    Scene *_scene;
    State *_state;
	Screen *_screen;
	Sound *_sound;
	Video *_video;

    void showMainMenu();
	void updateMainMenu();

};

} // namespace Asylum

#endif