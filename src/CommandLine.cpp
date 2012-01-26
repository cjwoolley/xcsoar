/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2012 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#include "CommandLine.hpp"
#include "Util/Args.hpp"
#include "Profile/Profile.hpp"
#include "OS/PathName.hpp"
#include "Simulator.hpp"

#include <windef.h> /* for MAX_PATH */

#if !defined(_WIN32_WCE)
int SCREENWIDTH = 640;
int SCREENHEIGHT = 480;
#endif

void ParseCommandLine(Args args)
{
  while (!args.IsEmpty()) {
    const char *s = args.GetNext();

    if (*s != '-')
      args.UsageError();

    // Also accept "--" prefix for arguments. Usually used on UNIX for long options
    if (s[1] == '-')
      s++;

#ifdef SIMULATOR_AVAILABLE
    if (strcmp(s, "-simulator") == 0) {
      global_simulator_flag = true;
      sim_set_in_cmd_line_flag = true;
      continue;
    }

    if (strcmp(s, "-fly") == 0) {
      global_simulator_flag=false;
      sim_set_in_cmd_line_flag=true;
      continue;
    }
#endif

    if (strncmp(s, "-profile=", 9) == 0) {
      s += 9;
      PathName convert(s);
      Profile::SetFiles(convert);
      continue;
    }

#if !defined(_WIN32_WCE)
    if (isdigit(s[1])) {
      char *p;
      SCREENWIDTH = strtol(s+1, &p, 10);
      if (*p != 'x' && *p != 'X')
        args.UsageError();
      s = p;
      SCREENHEIGHT = strtol(s+1, &p, 10);
      continue;
    }

    if (strcmp(s, "-portrait") == 0) {
      SCREENWIDTH = 480;
      SCREENHEIGHT = 640;
      continue;
    }

    if (strcmp(s, "-square") == 0) {
      SCREENWIDTH = 480;
      SCREENHEIGHT = 480;
      continue;
    }

    if (strcmp(s, "-small") == 0) {
      SCREENWIDTH = 320;
      SCREENHEIGHT = 240;
      continue;
    }
#endif

#if defined(_WIN32) && !defined(_WIN32_WCE) && !defined(__WINE__)
    if (strcmp(s, "-console") == 0) {
      AllocConsole();
      freopen("CONOUT$", "wb", stdout);
      continue;
    }
#endif

    args.UsageError();
  }

#if !defined(_WIN32_WCE)
  if (SCREENWIDTH < 240 || SCREENWIDTH > 4096 ||
      SCREENHEIGHT < 240 || SCREENHEIGHT > 4096)
    args.UsageError();
#endif
}
