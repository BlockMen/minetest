/*
Minetest
Copyright (C) 2013 celeron55, Perttu Ahola <celeron55@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "guiPauseMenu.h"
#include "debug.h"
#include "serialization.h"
#include "porting.h"
#include "config.h"
#include "main.h"
#include <IGUICheckBox.h>
#include <IGUIEditBox.h>
#include <IGUIButton.h>
#include <IGUIStaticText.h>
#include <IGUIFont.h>
#include "gettext.h"
#include "util/string.h"
#include "settings.h"
#if USE_FREETYPE
#include "xCGUITTFont.h"
#endif

extern bool paused;

GUIPauseMenu::GUIPauseMenu(gui::IGUIEnvironment* env,
		gui::IGUIElement* parent, s32 id,
		IGameCallback *gamecallback,
		IMenuManager *menumgr,
		bool simple_singleplayer_mode):
	GUIModalMenu(env, parent, id, menumgr),
	m_gamecallback(gamecallback),
	m_simple_singleplayer_mode(simple_singleplayer_mode)
{
}

GUIPauseMenu::~GUIPauseMenu()
{
	removeChildren();
}

void GUIPauseMenu::removeChildren()
{
	{
		gui::IGUIElement *e = getElementFromId(256);
		if(e != NULL)
			e->remove();
	}
	{
		gui::IGUIElement *e = getElementFromId(257);
		if(e != NULL)
			e->remove();
	}
	{
		gui::IGUIElement *e = getElementFromId(258);
		if(e != NULL)
			e->remove();
	}
	{
		gui::IGUIElement *e = getElementFromId(259);
		if(e != NULL)
			e->remove();
	}
	{
		gui::IGUIElement *e = getElementFromId(260);
		if(e != NULL)
			e->remove();
	}
	{
		gui::IGUIElement *e = getElementFromId(261);
		if(e != NULL)
			e->remove();
	}
	{
		gui::IGUIElement *e = getElementFromId(262);
		if(e != NULL)
			e->remove();
	}
}

void GUIPauseMenu::regenerateGui(v2u32 screensize)
{
	if(m_simple_singleplayer_mode)
		paused = true;
	/*
		Remove stuff
	*/
	removeChildren();
	
	/*
		Calculate new sizes and positions
	*/
	core::rect<s32> rect(
			screensize.X/2 - 580/2,
			screensize.Y/2 - 300/2,
			screensize.X/2 + 580/2,
			screensize.Y/2 + 300/2
	);
	
	DesiredRect = rect;
	recalculateAbsolutePosition(false);

	v2s32 size = rect.getSize();

	/*
		Add stuff
	*/
	const s32 btn_height = 30;
	const s32 btn_gap = 20;
	const s32 btn_num = 4;
	s32 btn_y = size.Y/2-((btn_num*btn_height+(btn_num-1)*btn_gap))/2-30;
	btn_y += btn_height + btn_gap;
	changeCtype("");
	{
		core::rect<s32> rect(0, 0, 290, btn_height*1.3);
		rect = rect + v2s32(size.X/2-290/2, btn_y);
		wchar_t* text = wgettext("Continue");
		Environment->addButton(rect, this, 256,	text);
		delete[] text;
	}
	btn_y += btn_height + btn_gap*1.3;
	u32 btn_w = 290;
	u32 btn_off = btn_w/2;
	if(!m_simple_singleplayer_mode)
	{
		btn_w = 140;
		btn_off = 145;
		{
			core::rect<s32> rect(0, 0, 140, btn_height);
			rect = rect + v2s32(size.X/2+5, btn_y);
			wchar_t* text = wgettext("Change Password");
			Environment->addButton(rect, this, 261,	text);
			delete[] text;
		}
	}
	{
		core::rect<s32> rect(0, 0, btn_w, btn_height);
		rect = rect + v2s32(size.X/2-btn_off, btn_y);
		wchar_t* text = wgettext("Settings");
		Environment->addButton(rect, this, 262, text);
		delete[] text;
	}
	btn_y += btn_height + btn_gap;
	{
		core::rect<s32> rect(0, 0, 140, btn_height);
		rect = rect + v2s32(size.X/2-145, btn_y);
		wchar_t* text = wgettext("Exit to Menu");
		Environment->addButton(rect, this, 260,	text);
		delete[] text;
	}
	{
		core::rect<s32> rect(0, 0, 140, btn_height);
		rect = rect + v2s32(size.X/2+5, btn_y);
		wchar_t* text = wgettext("Exit to OS");
		Environment->addButton(rect, this, 257,
			text);
		delete[] text;
	}

	/*{
		core::rect<s32> rect(0, 0, 180, 240);
		rect = rect + v2s32(size.X/2 + 90, size.Y/2-rect.getHeight()/2);
		wchar_t* text = wgettext("Default Controls:\n"
				"- WASD: move\n"
				"- Space: jump/climb\n"
				"- Shift: sneak/go down\n"
				"- Q: drop item\n"
				"- I: inventory\n"
				"- Mouse: turn/look\n"
				"- Mouse left: dig/punch\n"
				"- Mouse right: place/use\n"
				"- Mouse wheel: select item\n"
				"- T: chat\n"
				);
		Environment->addStaticText(text, rect, false, true, this, 258);
		delete[] text;

	}
	{
		core::rect<s32> rect(0, 0, 180, 220);
		rect = rect + v2s32(size.X/2 - 90 - rect.getWidth(), size.Y/2-rect.getHeight()/2);
	
		v2u32 max_texture_size;
		{
			video::IVideoDriver* driver = Environment->getVideoDriver();
			max_texture_size = driver->getMaxTextureSize();
		}

		std::ostringstream os;
		os<<"Minetest\n";
		os<<BUILD_INFO<<"\n";
		os<<"path_user = "<<wrap_rows(porting::path_user, 20)<<"\n";
	
		Environment->addStaticText(narrow_to_wide(os.str()).c_str(), rect, false, true, this, 259);
	}*/
	changeCtype("C");
}

void GUIPauseMenu::drawMenu()
{
	gui::IGUISkin* skin = Environment->getSkin();
	if (!skin)
		return;
	video::IVideoDriver* driver = Environment->getVideoDriver();

	video::SColor bgcolor(140,0,0,0);
	v2u32 screenSize = driver->getScreenSize();
	core::rect<s32> allbg(0, 0, screenSize.X ,	screenSize.Y);

	driver->draw2DRectangle(bgcolor, allbg, &allbg);

	gui::IGUIElement::draw();

	std::string font_path = g_settings->get("font_path");
	gui::IGUIFont *font;
	bool use_freetype = g_settings->getBool("freetype");
	#if USE_FREETYPE
	if (use_freetype) {
		u16 font_size = g_settings->getU16("font_size");
		font_size *= 2;
		font = gui::CGUITTFont::createTTFont(Environment, font_path.c_str(), font_size);
	} else {
		font = Environment->getFont(font_path.c_str());
	}
	#else
		font = guienv->getFont(font_path.c_str());
	#endif
	
	irr::core::dimension2d<u32> textsize = font->getDimension(L"Pause Menu");
	font->draw(L"Pause Menu",core::rect<s32>((driver->getScreenSize().Width - textsize.Width)/2,driver->getScreenSize().Height/3-20,textsize.Width,textsize.Height),video::SColor(255,255,255,255),false,false);

	delete[] font;
}

bool GUIPauseMenu::OnEvent(const SEvent& event)
{

	if(event.EventType==EET_KEY_INPUT_EVENT)
	{
		if(event.KeyInput.PressedDown)
		{
			if(event.KeyInput.Key==KEY_ESCAPE)
			{
				paused = false;
				quitMenu();
				return true;
			}
			else if(event.KeyInput.Key==KEY_RETURN)
			{
				paused = false;
				quitMenu();
				return true;
			}
		}
	}
	if(event.EventType==EET_GUI_EVENT)
	{
		if(event.GUIEvent.EventType==gui::EGET_ELEMENT_FOCUS_LOST
				&& isVisible())
		{
			if(!canTakeFocus(event.GUIEvent.Element))
			{
				dstream<<"GUIPauseMenu: Not allowing focus change."
						<<std::endl;
				// Returning true disables focus change
				return true;
			}
		}
		if(event.GUIEvent.EventType==gui::EGET_BUTTON_CLICKED)
		{
			switch(event.GUIEvent.Caller->getID())
			{
			case 256: // continue
				paused = false;
				quitMenu();
				// ALWAYS return immediately after quitMenu()
				return true;
			case 261:
				paused = false;
				quitMenu();
				m_gamecallback->changePassword();
				return true;
			case 262:
				paused = false;
				quitMenu();
				m_gamecallback->changeVolume();
				return true;
			case 260: // disconnect
				m_gamecallback->disconnect();
				paused = false;
				quitMenu();
				return true;
			case 257: // exit
				m_gamecallback->exitToOS();
				paused = false;
				quitMenu();
				return true;
			}
		}
	}
	
	return Parent ? Parent->OnEvent(event) : false;
}

