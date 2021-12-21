// SPDX-License-Identifier: GPL-2.0+

#pragma once

#define DIVS_TIME 10.0   ///< Number of horizontal screen divs
#define DIVS_VOLTAGE 8.0 ///< Number of vertical screen divs
#define DIVS_SUB 5       ///< Number of sub-divisions per div

#define MARGIN_LEFT ( -DIVS_TIME / 2.0 )
#define MARGIN_RIGHT ( DIVS_TIME / 2.0 )
#define MARGIN_TOP ( DIVS_VOLTAGE / 2.0 )
#define MARGIN_BOTTOM ( -DIVS_VOLTAGE / 2.0 )

#define MARKER_STEP ( DIVS_TIME / 100.0 )

// where are the (local) documents?
#ifdef __FreeBSD__
#define USR_MANUAL_PATH "/usr/local/share/doc/openhantek/OpenHantek6022_User_Manual.pdf"
#define AC_MODIFICATION_PATH "/usr/local/share/doc/openhantek/HANTEK6022_AC_Modification.pdf"
#else
#define USR_MANUAL_PATH "/usr/share/doc/openhantek/OpenHantek6022_User_Manual.pdf"
#define AC_MODIFICATION_PATH "/usr/share/doc/openhantek/HANTEK6022_AC_Modification.pdf"
#endif
