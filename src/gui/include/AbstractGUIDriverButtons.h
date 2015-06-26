/*                                              -*- mode:C++ -*-
  AbstractGUIDriverButtons.h Buttons for the AbstractGUIDriver
  Copyright (C) 2014-2015 The Regents of the University of New Mexico.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
  USA
*/

/**
  \file AbstractGUIDriverButtons.h Buttons for the AbstractGUIDriver
  \author Trent R. Small.
  \author David H. Ackley.
  \date (C) 2014-2015 All rights reserved.
  \lgpl
 */
#ifndef ABSTRACTGUIDRIVERBUTTONS_H
#define ABSTRACTGUIDRIVERBUTTONS_H

#include "Grid.h"
#include "TileRenderer.h"
#include "AbstractButton.h"
#include "AbstractCheckbox.h"
#include "Camera.h"

namespace MFM
{
  template<class GC> class AbstractGUIDriver; // FORWARD
  template<class GC> class AbstractDriver; // FORWARD

  template<class GC>
  class HasGUIDriver {
  public:
    typedef typename GC::EVENT_CONFIG EC;
    typedef TileRenderer<EC> OurTileRenderer;

    HasGUIDriver()
      : m_driver(0)
    { }

    ~HasGUIDriver() { }

    virtual AbstractButton  & GetAbstractButton() = 0;

    void SetDriver(AbstractGUIDriver<GC> & driver)
    {
      m_driver = &driver;
    }

    AbstractGUIDriver<GC> & GetDriver()
    {
      if (!m_driver) FAIL(ILLEGAL_STATE);
      return *m_driver;
    }

    const AbstractGUIDriver<GC> & GetDriver() const
    {
      if (!m_driver) FAIL(ILLEGAL_STATE);
      return *m_driver;
    }

    Grid<GC> & GetGrid() { return GetDriver().GetGrid(); }
    const Grid<GC> & GetGrid() const { return GetDriver().GetGrid(); }

    OurTileRenderer & GetTileRenderer() { return this->GetDriver().GetTileRenderer(); }
    const OurTileRenderer & GetTileRenderer() const { return this->GetDriver().GetTileRenderer(); }

  private:
    AbstractGUIDriver<GC> * m_driver;
  };

  template<class GC>
  class AbstractGridButton : public AbstractButton, public HasGUIDriver<GC>
  {
  public:

    virtual AbstractButton & GetAbstractButton()
    {
      return *this;
    }

    AbstractGridButton(const char* title)
      : AbstractButton(title)
    { }

  };

  template<class GC>
  class AbstractGridCheckbox : public AbstractCheckbox, public HasGUIDriver<GC>
  {
  public:

    AbstractGridCheckbox(const char* title)
      : AbstractCheckbox(title)
    {
    }

    virtual AbstractButton & GetAbstractButton()
    {
      return *this;
    }

    virtual void OnCheck(bool value)
    {
      this->SetChecked(value);
    }
  };



  /************************************/
  /******CONCRETE BUTTONS**************/
  /************************************/

  template<class GC>
  struct ClearButton : public AbstractGridButton<GC>
  {
    ClearButton()
      : AbstractGridButton<GC>("Clear Tile")
    {
      AbstractButton::SetName("ClearButton");
      Panel::SetDoc("Clear selected tile if any");
      Panel::SetFont(FONT_ASSET_BUTTON_SMALL);
    }

    virtual bool GetKeyboardAccelerator(u32 & keysym, u32 & mod)
    {
      keysym = SDLK_t;
      mod = KMOD_CTRL|KMOD_SHIFT;
      return true;
    }

    virtual void OnClick(u8 button)
    {
      FAIL(INCOMPLETE_CODE);
#if 0
      GridRenderer & grend = this->GetGridRenderer();

      const SPoint selTile = grend.GetSelectedTile();
      if(selTile.GetX() >= 0 && selTile.GetX() < this->GetGrid().GetWidth() &&
         selTile.GetY() >= 0 && selTile.GetY() < this->GetGrid().GetHeight())
      {
        this->GetGrid().EmptyTile(grend.GetSelectedTile());
      }
#endif
    }

  };

  template<class GC>
  struct ClearGridButton : public AbstractGridButton<GC>
  {
    ClearGridButton()
      : AbstractGridButton<GC>("Clear Grid")
    {
      AbstractButton::SetName("ClearGridButton");
      Panel::SetDoc("Clear entire grid");
      Panel::SetFont(FONT_ASSET_BUTTON_MEDIUM);
    }

    virtual bool GetKeyboardAccelerator(u32 & keysym, u32 & mod)
    {
      keysym = SDLK_g;
      mod = KMOD_CTRL|KMOD_SHIFT;
      return true;
    }

    virtual void OnClick(u8 button)
    {
      this->GetGrid().Clear();
    }

  };

  template<class GC>
  struct NukeButton : public AbstractGridButton<GC>
  {
    NukeButton()
      : AbstractGridButton<GC>("Nuke")
    {
      AbstractButton::SetName("NukeButton");
      Panel::SetDoc("Clear a random-sized circle at a random location");
      Panel::SetFont(FONT_ASSET_BUTTON_MEDIUM);
    }

    virtual bool GetKeyboardAccelerator(u32 & keysym, u32 & mod)
    {
      keysym = SDLK_n;
      mod = KMOD_CTRL;
      return true;
    }

    virtual void OnClick(u8 button)
    {
      this->GetGrid().RandomNuke();
    }
  };

  template<class GC>
  struct ThinButton : public AbstractGridButton<GC>
  {
    ThinButton()
      : AbstractGridButton<GC>("Thin")
    {
      AbstractButton::SetName("ThinButton");
      Panel::SetDoc("Set 1% of sites empty");
      Panel::SetFont(FONT_ASSET_BUTTON_MEDIUM);
    }

    virtual bool GetKeyboardAccelerator(u32 & keysym, u32 & mod)
    {
      keysym = SDLK_t;
      mod = KMOD_CTRL;
      return true;
    }

    virtual void OnClick(u8 button)
    {
      this->GetGrid().Thin();
    }
  };

  template<class GC>
  struct XRayButton : public AbstractGridButton<GC>
  {
    XRayButton()
      : AbstractGridButton<GC>("XRay")
    {
      AbstractButton::SetName("XRayButton");
      Panel::SetDoc("In 1% of sites, flip 1% of bits");
      Panel::SetFont(FONT_ASSET_BUTTON_MEDIUM);
    }

    virtual bool GetKeyboardAccelerator(u32 & keysym, u32 & mod)
    {
      keysym = SDLK_x;
      mod = 0;
      return true;
    }

    virtual void OnClick(u8 button)
    {
      this->GetGrid().XRay();
    }

  };

  template<class GC>
  struct GridRunCheckbox : public AbstractGridCheckbox<GC>
  {
    GridRunCheckbox()
      : AbstractGridCheckbox<GC>("Pause")
    {
      AbstractButton::SetName("GridRunButton");
      Panel::SetDoc("Pause/run the grid");
      Panel::SetFont(FONT_ASSET_BUTTON_BIG);
    }

    virtual bool GetKeyboardAccelerator(u32 & keysym, u32 & mod)
    {
      keysym = SDLK_SPACE;
      mod = 0;
      return true;
    }

    virtual bool IsChecked() const
    {
      return this->GetDriver().IsKeyboardPaused();
    }
    virtual void SetChecked(bool checked)
    {
      this->GetDriver().SetKeyboardPaused(checked);
    }

  };

  template<class GC>
  struct GridRenderButton : public AbstractGridCheckbox<GC>
  {
    GridRenderButton()
      : AbstractGridCheckbox<GC>("Grid")
    {
      AbstractButton::SetName("GridRenderButton");
      Panel::SetDoc("Do/don't draw grid of lines around the sites");
      Panel::SetFont(FONT_ASSET_BUTTON_MEDIUM);
    }

    virtual bool GetKeyboardAccelerator(u32 & keysym, u32 & mod)
    {
      keysym = SDLK_g;
      mod = 0;
      return true;
    }

    virtual bool IsChecked() const
    {
      return this->GetTileRenderer().IsDrawGrid();
    }

    virtual void SetChecked(bool value)
    {
      this->GetTileRenderer().SetDrawGrid(value);
    }
  };

  template<class GC>
  struct CacheRenderButton : public AbstractGridCheckbox<GC>
  {
    CacheRenderButton()
      : AbstractGridCheckbox<GC>("Cache")
    {
      AbstractButton::SetName("CacheRenderButton");
      Panel::SetDoc("Do/don't include cache sites around the tiles");
      Panel::SetFont(FONT_ASSET_BUTTON_MEDIUM);
    }

    virtual bool GetKeyboardAccelerator(u32 & keysym, u32 & mod)
    {
      keysym = SDLK_c;
      mod = 0;
      return true;
    }

    virtual bool IsChecked() const
    {
      return this->GetTileRenderer().IsDrawCaches();
    }

    virtual void SetChecked(bool value)
    {
      this->GetTileRenderer().SetDrawCaches(value);
    }
  };

  template<class GC>
  struct LoadGridSectionButton : public AbstractGridCheckbox<GC>
  {
    LoadGridSectionButton()
      : AbstractGridCheckbox<GC>("Grid")
    {
      AbstractButton::SetName("LoadGridSectionButton");
      Panel::SetDoc("Do/don't include [GRID] section when loading");
      Panel::SetFont(FONT_ASSET_BUTTON_SMALL);
    }

    virtual bool GetKeyboardAccelerator(u32 & keysym, u32 & mod)
    {
      return false;
    }

    virtual bool IsChecked() const
    {
      return this->GetDriver().IsLoadGridSection();
    }

    virtual void SetChecked(bool value)
    {
      this->GetDriver().SetLoadGridSection(value);
    }
  };

  template<class GC>
  struct LoadGUISectionButton : public AbstractGridCheckbox<GC>
  {
    LoadGUISectionButton()
      : AbstractGridCheckbox<GC>("GUI")
    {
      AbstractButton::SetName("LoadGUISectionButton");
      Panel::SetDoc("Do/don't include [GUI] section when loading");
      Panel::SetFont(FONT_ASSET_BUTTON_SMALL);
    }

    virtual bool GetKeyboardAccelerator(u32 & keysym, u32 & mod)
    {
      return false;
    }

    virtual bool IsChecked() const
    {
      return this->GetDriver().IsLoadGUISection();
    }

    virtual void SetChecked(bool value)
    {
      this->GetDriver().SetLoadGUISection(value);
    }
  };

  template<class GC>
  struct LoadDriverSectionButton : public AbstractGridCheckbox<GC>
  {
    LoadDriverSectionButton()
      : AbstractGridCheckbox<GC>("Driver")
    {
      AbstractButton::SetName("LoadDriverSectionButton");
      Panel::SetDoc("Do/don't include [DRIVER] section when loading");
      Panel::SetFont(FONT_ASSET_BUTTON_SMALL);
    }

    virtual bool GetKeyboardAccelerator(u32 & keysym, u32 & mod)
    {
      return false;
    }

    virtual bool IsChecked() const
    {
      return this->GetDriver().IsLoadDriverSection();
    }

    virtual void SetChecked(bool value)
    {
      this->GetDriver().SetLoadDriverSection(value);
    }
  };

  template<class GC>
  struct FgViewButton : public AbstractGridButton<GC>
  {
    FgViewButton()
      : AbstractGridButton<GC>("Front: Atom #1")
    {
      AbstractButton::SetName("FgViewButton");
      Panel::SetDoc("Change atom ('front') rendering method");
      Panel::SetFont(FONT_ASSET_BUTTON_BIG);
    }

    virtual bool GetKeyboardAccelerator(u32 & keysym, u32 & mod)
    {
      keysym = SDLK_f;
      mod = 0;
      return true;
    }

    virtual void OnClick(u8 button)
    {
      this->GetTileRenderer().NextDrawForegroundType();
      UpdateLabel();
    }

    void UpdateLabel()
    {
      OString32 fgText;
      fgText.Printf("Front: %s",
                    this->GetTileRenderer().GetDrawForegroundTypeName());
      AbstractButton::SetText(fgText.GetZString());
    }

  };

  template<class GC>
  struct GridStepCheckbox : public AbstractGridButton<GC>
  {
    GridStepCheckbox()
      : AbstractGridButton<GC>("Step")
    {
      AbstractButton::SetName("GridStepButton");
      Panel::SetDoc("'Single' step (~1 AEPS) the grid");
      Panel::SetFont(FONT_ASSET_BUTTON_BIG);
    }

    virtual bool GetKeyboardAccelerator(u32 & keysym, u32 & mod)
    {
      keysym = SDLK_s;
      mod = 0;
      return true;
    }

    virtual void OnClick(u8 button)
    {
      this->GetDriver().SetSingleStep(true);
      this->GetDriver().SetKeyboardPaused(false);
    }
  };

  template<class GC>
  struct BgViewButton : public AbstractGridButton<GC>
  {
    BgViewButton()
      : AbstractGridButton<GC>("Back: Base #1")
    {
      AbstractButton::SetName("BgViewButton");
      Panel::SetDoc("Change site ('back') rendering method");
      Panel::SetFont(FONT_ASSET_BUTTON_BIG);
    }

    virtual bool GetKeyboardAccelerator(u32 & keysym, u32 & mod)
    {
      keysym = SDLK_b;
      mod = 0;
      return true;
    }

    virtual void OnClick(u8 button)
    {
      this->GetTileRenderer().NextDrawBackgroundType();
      UpdateLabel();
    }

    void UpdateLabel()
    {
      OString32 label;
      label.Printf("Back: %s",
                    this->GetTileRenderer().GetDrawBackgroundTypeName());
      AbstractButton::SetText(label.GetZString());
    }
  };

  template<class GC>
  struct SaveButton : public AbstractGridButton<GC>
  {
    SaveButton()
      : AbstractGridButton<GC>("Save")
    {
      AbstractButton::SetName("SaveButton");
      Panel::SetDoc("Save simulation state in next filename");
      Panel::SetFont(FONT_ASSET_BUTTON_BIG);
    }
    virtual bool GetKeyboardAccelerator(u32 & keysym, u32 & mod)
    {
      keysym = SDLK_s;
      mod = KMOD_CTRL;
      return true;
    }
    virtual void OnClick(u8 button)
    {
      this->GetDriver().SaveGridWithNextFilename();
    }
  };

  template<class GC>
  struct ScreenshotButton : public AbstractGridButton<GC>
  {
  public:
    ScreenshotButton()
      : AbstractGridButton<GC>("Picture")
      , m_currentScreenshot(0)
      , m_screen(0)
      , m_camera(0)
    {
      AbstractButton::SetName("Picture");
      Panel::SetDoc("Save screenshot in next filename");
      Panel::SetFont(FONT_ASSET_BUTTON_BIG);
    }

    virtual bool GetKeyboardAccelerator(u32 & keysym, u32 & mod)
    {
      keysym = SDLK_p;
      mod = 0;
      return true;
    }

    void SetScreen(SDL_Surface* screen)
    {
      m_screen = screen;
    }

    void SetCamera(Camera* camera)
    {
      m_camera = camera;
    }

    virtual void OnClick(u8 button)
    {
      if(m_screen && m_camera)
      {
        const u32 aeps = this->GetDriver().GetThisEpochAEPS();
        const char * path =
          this->GetDriver().GetSimDirPathTemporary("screenshot/%D-%D.png",
                                                   aeps,
                                                   ++m_currentScreenshot);
        m_camera->DrawSurface(m_screen, path);

        LOG.Message("Screenshot saved at %s", path);
      }
      else
      {
        LOG.Debug("Screenshot not saved; screen is null. Use SetScreen() first.");
      }
    }

  private:
    u32 m_currentScreenshot;
    SDL_Surface* m_screen;
    Camera* m_camera;
    AbstractDriver<GC>* m_driver;
  };

  template<class GC>
  struct QuitButton : public AbstractGridButton<GC>
  {
    QuitButton()
      : AbstractGridButton<GC>("Quit")
    {
      AbstractButton::SetName("QuitButton");
      Panel::SetDoc("Exit simulation");
      Panel::SetFont(FONT_ASSET_BUTTON_BIG);
    }

    virtual bool GetKeyboardAccelerator(u32 & keysym, u32 & mod)
    {
      keysym = SDLK_q;
      mod = KMOD_CTRL;
      return true;
    }

    virtual void OnClick(u8 button)
    {
      exit(0);
    }
  };

  template<class GC>
  struct ReloadButton : public AbstractGridButton<GC>
  {
    ReloadButton()
      : AbstractGridButton<GC>("Load")
    {
      AbstractButton::SetName("ReloadButton");
      Panel::SetDoc("Load (next) -cp saved file from command line");
      Panel::SetFont(FONT_ASSET_BUTTON_BIG);
    }

    virtual bool GetKeyboardAccelerator(u32 & keysym, u32 & mod)
    {
      keysym = SDLK_l;
      mod = KMOD_CTRL;
      return true;
    }

    virtual void OnClick(u8 button)
    {
      this->GetDriver().LoadFromConfigurationPath();
    }
  };

  template<class GC>
  struct PauseTileButton : public AbstractGridButton<GC>
  {
    PauseTileButton()
      : AbstractGridButton<GC>("Pause Tile")
    {
      AbstractButton::SetName("PauseTileButton");
      Panel::SetDoc("Pause/unpause selected tile if any");
      Panel::SetFont(FONT_ASSET_BUTTON_SMALL);
    }

    virtual bool GetKeyboardAccelerator(u32 & keysym, u32 & mod)
    {
      keysym = SDLK_p;
      mod = KMOD_CTRL;
      return true;
    }

    virtual void OnClick(u8 button)
    {
      FAIL(INCOMPLETE_CODE);
#if 0
      SPoint selectedTile = this->GetGridRenderer().GetSelectedTile();

      if(selectedTile.GetX() >= 0 && selectedTile.GetY() >= 0)
      {
        bool isEnabled = this->GetGrid().IsTileEnabled(selectedTile);
        this->GetGrid().SetTileEnabled(selectedTile, !isEnabled);
      }
#endif
    }
  };

  template<class GC>
  struct BGRButton : public AbstractGridCheckbox<GC>
  {
    BGRButton()
      : AbstractGridCheckbox<GC>("Writes fault")
    {
      AbstractButton::SetName("BGRButton");
      Panel::SetDoc("In 1% of site updates, do/don't flip 1% of bits");
      Panel::SetFont(FONT_ASSET_BUTTON_SMALL);
    }

    virtual bool GetKeyboardAccelerator(u32 & keysym, u32 & mod)
    {
      keysym = SDLK_x;
      mod = KMOD_CTRL;
      return true;
    }

    virtual bool IsChecked() const
    {
      return this->GetGrid().IsBackgroundRadiationEnabled();
    }

    virtual void SetChecked(bool value)
    {
      this->GetGrid().SetBackgroundRadiationEnabled(value);
    }

  };

  template<class GC>
  struct FGRButton : public AbstractGridCheckbox<GC>
  {
    FGRButton()
      : AbstractGridCheckbox<GC>("Reads fault")
    {
      AbstractButton::SetName("FGRButton");
      Panel::SetDoc("In 1% of site reads, do/don't flip 1% of bits");
      Panel::SetFont(FONT_ASSET_BUTTON_SMALL);
      AbstractButton::SetEnabled(false);
    }

    virtual bool GetKeyboardAccelerator(u32 & keysym, u32 & mod)
    {
      keysym = SDLK_x;
      mod = KMOD_CTRL|KMOD_SHIFT;
      return true;
    }

    virtual bool IsChecked() const
    {
      return this->GetGrid().IsForegroundRadiationEnabled();
    }

    virtual void SetChecked(bool value)
    {
      this->GetGrid().SetForegroundRadiationEnabled(value);
    }

  };

  template<class GC>
  struct LogButton : public AbstractGridCheckbox<GC>
  {
    LogButton()
      : AbstractGridCheckbox<GC>("Show log")
    {
      AbstractButton::SetName("LogButton");
      Panel::SetDoc("Do/don't show the log window");
      Panel::SetFont(FONT_ASSET_BUTTON_MEDIUM);
    }
    virtual bool GetKeyboardAccelerator(u32 & keysym, u32 & mod)
    {
      keysym = SDLK_l;
      mod = 0;
      return true;
    }

    virtual bool IsChecked() const
    {
      return  this->GetDriver().IsLogVisible();
    }

   virtual void SetChecked(bool value)
    {
      this->GetDriver().SetLogVisible(value);
    }

  };

  template<class GC>
  struct ShowHelpButton : public AbstractGridCheckbox<GC>
  {
    ShowHelpButton()
      : AbstractGridCheckbox<GC>("Show help")
    {
      AbstractButton::SetName("ShowHelpButton");
      Panel::SetDoc("Do/don't show the help window");
      Panel::SetFont(FONT_ASSET_BUTTON_BIG);
    }
    virtual bool GetKeyboardAccelerator(u32 & keysym, u32 & mod)
    {
      keysym = SDLK_h;
      mod = 0;
      return true;
    }
    virtual bool IsChecked() const
    {
      return this->GetDriver().IsHelpVisible();
    }
    virtual void SetChecked(bool value)
    {
      this->GetDriver().SetHelpVisible(value);
    }

  };

  template<class GC>
  struct ShowToolboxButton : public AbstractGridCheckbox<GC>
  {
    ShowToolboxButton()
      : AbstractGridCheckbox<GC>("Show tools")
    {
      AbstractButton::SetName("ToolboxButton");
      Panel::SetDoc("Do/don't show the toolbox window");
      Panel::SetFont(FONT_ASSET_BUTTON_BIG);
    }

    virtual bool GetKeyboardAccelerator(u32 & keysym, u32 & mod)
    {
      keysym = SDLK_t;
      mod = 0;
      return true;
    }

    virtual bool IsChecked() const
    {
      return this->GetDriver().IsToolboxVisible();
    }

    virtual void SetChecked(bool value)
    {
      this->GetDriver().SetToolboxVisible(value);
    }
  };

  template<class GC>
  struct ShowInfoBoxButton : public AbstractGridCheckbox<GC>
  {
    ShowInfoBoxButton()
      : AbstractGridCheckbox<GC>("Show info")
    {
      AbstractButton::SetName("InfoButton");
      Panel::SetDoc("Do/don't show the statistics info window");
      Panel::SetFont(FONT_ASSET_BUTTON_BIG);
    }
    virtual bool GetKeyboardAccelerator(u32 & keysym, u32 & mod)
    {
      keysym = SDLK_i;
      mod = 0;
      return true;
    }

    virtual bool IsChecked() const
    {
      return  this->GetDriver().IsInfoBoxVisible();
    }

   virtual void SetChecked(bool value)
    {
      this->GetDriver().SetInfoBoxVisible(value);
    }

  };


} /* namespace MFM */

#endif /* ABSTRACTGUIDRIVERBUTTONS_H */
