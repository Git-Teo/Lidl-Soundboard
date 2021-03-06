#ifndef HOOKCONTROLLER_H
#define HOOKCONTROLLER_H

#include "qt_windows.h"
#include "winuser.h"
#include <QVector>
#include <QDebug>
#include "OverlayController.h"
#include "Profile.h"
#include "windows.h"
#include <vector>
#include "psapi.h" // GetModuleFileNameExA or whatever
#include "string"
#include <QFileInfo>
#include "ProfileController.h"

/*!
 * \file HookController.h
 * \brief File handling the Hook Controller Singleton as well as the definition of the CallBack
 *
 * \author Devolution
 * \version 1.0.0
 * \since 1.9.0
 *
 */


namespace LIDL {

namespace Controller {

/*!
 * \brief The HookController class
 *
 * The singleton dealing with registering and unregistering the WinApi hooks
 */
class HookController
{

public:
    /*!
     * \brief GetInstace
     * \return
     */
    static HookController * GetInstance();



    /*!
     * \brief SetHooks
     * Sets up the hooks
     */
    void SetHooks();

    /*!
     * \brief UnSetHooks
     * Unregister the hooks (before closing app, because if you don't unregister them
     * close the app, and switch to another window fast it will trigger a
     * "must construct QApplication before QPixMap" error and crash
     * because of the overlay still trying to show
     */
    void UnSetHooks();

    /*!
     * \brief GetProfileForExe
     * \return A pointer to the correct profile, or nullptr if no profile was found !
     *
     * Basically will: Get the full path to the executable linked to the window
     * Transform it to linux style path with forward slash
     * Search for it in the array of profile using the designated method in SettingsController
     */
    static Profile * GetProfileForExe(HWND handle);


private:
    static HookController * self;
    HookController();

    QVector<HWINEVENTHOOK> _hookHandles;


};
} // end namespace Controller

namespace Callback{
/*!
     * \brief ResizeToWindow
     * \param hook
     * \param event
     * \param hwnd
     * \param idObject
     * \param idChild
     * \param dwEventThread
     * \param dwmsEventTime
     *
     * Used to call LIDL::OverlayController::GetInstance()->ResizeToWindow(GetForegroundWindow());
     * On:  EVENT_SYSTEM_MOVESIZEEND
     * (GetForegroundWindow() is defined in winuser.h)
     */
    void CALLBACK ResizeToWindow(HWINEVENTHOOK hook, DWORD event, HWND hwnd,
                             LONG idObject, LONG idChild,
                             DWORD dwEventThread, DWORD dwmsEventTime);

    /*!
     * \brief ShowOverlay
     * \param hook
     * \param event
     * \param hwnd
     * \param idObject
     * \param idChild
     * \param dwEventThread
     * \param dwmsEventTime
     *
     * Used to call    LIDL::OverlayController::GetInstance()->ResizeToWindow(GetForegroundWindow());
     *                  LIDL::OverlayController::GetInstance()->ShowGameOverlay(GetForegroundWindow());
     * On:  EVENT_SYSTEM_MOVESIZEEND, EVENT_SYSTEM_MINIMIZEEND, EVENT_SYSTEM_MINIMIZESTART
     * (GetForegroundWindow() is defined in winuser.h)
     */
    void CALLBACK ShowOverlay(HWINEVENTHOOK hook, DWORD event, HWND hwnd,
                                 LONG idObject, LONG idChild,
                                 DWORD dwEventThread, DWORD dwmsEventTime);
}


} // end namespace LIDL




#endif // HOOKCONTROLLER_H
