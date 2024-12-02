/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include "mainComponent.h"
#include <JuceHeader.h>

//==============================================================================
class jelodyneApplication : public juce::JUCEApplication {
  public:
    //==============================================================================
    jelodyneApplication() {}

    const juce::String getApplicationName() override {
        return ProjectInfo::projectName;
    }
    const juce::String getApplicationVersion() override {
        return ProjectInfo::versionString;
    }
    bool moreThanOneInstanceAllowed() override { return true; }

    //==============================================================================
    void initialise(const juce::String &commandLine) override {
        // This method is where you should put your application's initialisation
        // code..

        juce::String appendedTitle = "";

        if (JUCE_DEBUG) {
            // https://forum.juce.com/t/cross-platform-build-number-or-date-access/21391/7
            juce::StringArray command;
            command.add("git");
            command.add("rev-parse");
            command.add("--short");
            command.add("HEAD");

            juce::ChildProcess c;
            juce::String gitHashShort;

            if (c.start(command, ChildProcess::wantStdOut)) {
                c.waitForProcessToFinish(1000);

                if (c.getExitCode() == 0)
                    gitHashShort = c.readAllProcessOutput().trim();
            }

            appendedTitle = "-dev-";
            appendedTitle.append(gitHashShort, 7);
        }

        mainWindow.reset(new MainWindow(getApplicationName() + " " +
                                        getApplicationVersion() +
                                        appendedTitle));
    }

    void shutdown() override {
        // Add your application's shutdown code here..

        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override {
        // This is called when the app is being asked to quit: you can ignore
        // this request and let the app carry on running, or call quit() to
        // allow the app to close.
        quit();
    }

    void anotherInstanceStarted(const juce::String &commandLine) override {
        // When another instance of the app is launched while this one is
        // running, this method is invoked, and the commandLine parameter tells
        // you what the other instance's command-line arguments were.
    }

    //==============================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainComponent class.
    */
    class MainWindow : public juce::DocumentWindow {
      public:
        MainWindow(juce::String name)
            : DocumentWindow(
                  name,
                  juce::Desktop::getInstance()
                      .getDefaultLookAndFeel()
                      .findColour(juce::ResizableWindow::backgroundColourId),
                  DocumentWindow::allButtons) {
            setUsingNativeTitleBar(true);
            setContentOwned(new MainComponent(), true);

#if JUCE_IOS || JUCE_ANDROID
            setFullScreen(true);
#else
            setResizable(true, true);
            centreWithSize(getWidth(), getHeight());
#endif

            setVisible(true);
        }

        void closeButtonPressed() override {
            // This is called when the user tries to close this window. Here,
            // we'll just ask the app to quit when this happens, but you can
            // change this to do whatever you need.
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the
           base class uses a lot of them, so by overriding you might break its
           functionality. It's best to do all your work in your content
           component instead, but if you really have to override any
           DocumentWindow methods, make sure your subclass also calls the
           superclass's method.
        */

      private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    };

  private:
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION(jelodyneApplication)
