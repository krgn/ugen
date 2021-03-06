/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic outline for a simple desktop window.

  ==============================================================================
*/

#include "MainWindow.h"
#include "MainComponent.h"

//==============================================================================
MainAppWindow::MainAppWindow()
: DocumentWindow (JUCEApplication::getInstance()->getApplicationName(),
                  Colours::lightgrey,
                  DocumentWindow::allButtons)
{
    setContentOwned(new MainComponent(), false);
    
	centreWithSize (400, 200);
    setVisible (true);
}

MainAppWindow::~MainAppWindow()
{
}

void MainAppWindow::closeButtonPressed()
{
    JUCEApplication::getInstance()->systemRequestedQuit();
}
