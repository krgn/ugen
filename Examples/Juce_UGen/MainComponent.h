#ifndef _MAINCOMPONENT_H_
#define _MAINCOMPONENT_H_


#include <juce/juce.h>
#include "../../UGen/UGen.h"



class MainComponent  :  public Component,
						public JuceIOHost,
						public ButtonListener,
						public SliderListener,
						public Timer
{
    //==============================================================================
    TextButton* audioSettingsButton;
	Label* cpuUsageLabel;
	
	Slider* freqSlider1;
	Slider* freqSlider2;
	Slider* ampSlider1;
	Slider* ampSlider2;
	TextButton* testButton1;
	TextButton* testButton2;
	ToggleButton* toggle1;
	ToggleButton* toggle2;
	ScopeComponent* scopeComponent;
	EnvelopeContainerComponent* envComponent;
	MultiSlider* sliders1;
	MultiSlider* sliders2;
	
	float freqValue1, freqValue2, ampValue1, ampValue2;
	
	UGen delayTime;
	UGen sender;
							
public:
	//==============================================================================
	MainComponent()
	:	JuceIOHost(1, 2, 1024, true)
    {		
		setName (T("UGen Test"));
						
		//==============================================================================
		
		addAndMakeVisible (audioSettingsButton = new TextButton (T("show audio settings..."),
																 T("click here to change the audio device settings")));
		audioSettingsButton->addListener (this);
		
		
		addAndMakeVisible(cpuUsageLabel = new Label(T("CPU usage"), T("0.00 %")));
		cpuUsageLabel->setJustificationType(Justification::right);
		
		addAndMakeVisible(freqSlider1 = new Slider(T("Frequency1")));
		freqSlider1->setRange(50, 5000, 0.0);
		freqSlider1->setSkewFactorFromMidPoint(250.0);
		freqSlider1->addListener(this);
		freqSlider1->setValue(100, true, true);
				
		addAndMakeVisible(freqSlider2 = new Slider(T("Frequency2")));
		freqSlider2->setRange(50, 5000, 0.0);
		freqSlider2->setSkewFactorFromMidPoint(250.0);
		freqSlider2->addListener(this);
		freqSlider2->setValue(100, true, true);
		
		addAndMakeVisible(ampSlider1 = new Slider(T("Amp1")));
		ampSlider1->setRange(0.0, 1.0, 0.0);
		ampSlider1->addListener(this);
		ampSlider1->setValue(0.5, true, true);
		
		addAndMakeVisible(ampSlider2 = new Slider(T("Amp2")));
		ampSlider2->setRange(0.0, 1.0, 0.0);
		ampSlider2->addListener(this);
		ampSlider2->setValue(0.5, true, true);
		
		addAndMakeVisible(testButton1 = new TextButton(T("Test Button 1")));
		testButton1->addListener(this);
		addAndMakeVisible(testButton2 = new TextButton(T("Test Button 2")));
		testButton2->addListener(this);
		
		addAndMakeVisible(toggle1 = new ToggleButton(T("Toggle 1")));
		toggle1->addListener(this);
		addAndMakeVisible(toggle2 = new ToggleButton(T("Toggle 2")));
		toggle2->addListener(this);
		
		addAndMakeVisible(sliders1 = new MultiSlider(4));
		addAndMakeVisible(sliders2 = new MultiSlider(4));
		
		addAndMakeVisible(scopeComponent = new ScopeComponent());
		scopeComponent->setScaleX(ScopeGUI::LabelXTime, 441, 2, false);
		scopeComponent->setScaleY(ScopeGUI::LabelYAmplitude);
		
		addAndMakeVisible(envComponent = new EnvelopeContainerComponent("My Envelope"));
		Env env = Env::linen(1.0, 1.0, 1.0, 0.7, EnvCurve::Sine);		
		envComponent->setDomainRange(env.duration());
		envComponent->setEnv(env);
		
		// add scope to the main out
		sender = Sender::AR(getOutput(), U(ampSlider2) / 10);
		sender.addBufferReceiver(scopeComponent);
		addOther(sender);
		
		// for CPU usage label
		startTimer(40);
	}
	
	~MainComponent()
	{
		stopTimer();
		
		sender.removeBufferReceiver(scopeComponent);
		
		deleteAllChildren();
	}
	
	void resized()
	{		
		audioSettingsButton->setBounds (10, 10, 200, 24);
		audioSettingsButton->changeWidthToFitText();
		
		cpuUsageLabel->setBounds(getWidth()-90, 10, 80, 24);
		
		freqSlider1->setBounds(10, 80, 200, 20);
		freqSlider2->setBounds(10, 110, 200, 20);
		ampSlider1->setBounds(10, 140, 200, 20);
		ampSlider2->setBounds(10, 170, 200, 20);
		
		testButton1->setBounds(10, 200, 200, 20);
		testButton2->setBounds(10, 230, 200, 20);
		
		toggle1->setBounds(10, 260, 200, 20);
		toggle2->setBounds(10, 290, 200, 20);
		
		sliders1->setBounds(10, 320, 200, 100);
		sliders2->setBounds(10, 450, 200, 100);
		
		scopeComponent->setBounds(230, 40, getWidth()-230-10, getHeight()-50-10-80);
		envComponent->setBounds(230, getHeight()-10-80, getWidth()-230-10, 80);
	}
	
	
	void buttonClicked (Button* button)
	{
		if (button == audioSettingsButton)
		{			
			// Create an AudioDeviceSelectorComponent which contains the audio choice widgets...
			AudioDeviceSelectorComponent audioSettingsComp (getAudioDeviceManager(),
															0, 2,
															2, 2,
															true,
															false,
															true,
															false);
			
			// ...and show it in a DialogWindow...
			audioSettingsComp.setSize (500, 400);
			
			DialogWindow::showModalDialog (T("Audio Settings"),
										   &audioSettingsComp,
										   this,
										   Colours::azure,
										   true);
		}
		else if(button == testButton1)
		{

		}
		else if(button == testButton2)
		{

		}
		else if(button == toggle1)
		{
			
		}
		else if(button == toggle2)
		{
			if(toggle1->getToggleState()) return;
				
			if(toggle2->getToggleState())
			{
				float start = UGen::getEstimatedBlockSize() * UGen::getReciprocalSampleRate();
				float end = 8;
				UGen newDelay = LLine::AR(start, end, end-start, UGen::DoNothing);
										  
				const ScopedLock sl(lock);
				delayTime.setSource(newDelay, true);
			}
			else
			{
				float newDelay = delayTime.getValue();
				const ScopedLock sl(lock);
				delayTime.setSource(newDelay, true);
			}
		}
	}
	
	void sliderValueChanged (Slider* slider)
	{		
		if(slider == freqSlider1)
		{
			freqValue1 = (float)freqSlider1->getValue();
		}
		else if(slider == freqSlider2)
		{
			freqValue2 = (float)freqSlider2->getValue();
		}
		else if(slider == ampSlider1)
		{
			ampValue1 = (float)ampSlider1->getValue();
		}
		else if(slider == ampSlider2)
		{
			ampValue2 = (float)ampSlider2->getValue();
		}
	}
	
	void timerCallback()
	{
		cpuUsageLabel->setText(String(getCpuUsage()*100.0, 2) + " %", false);
	}
			
	UGen constructGraph(UGen const& input)
	{				
		Value s = freqSlider1;
		UGen output = SinOsc::AR(s.kr(), 0, UGen(0.1, 0.1));
		return output * LLine::AR(0.0, 1.0, 1.0);
	}
};

#endif//_MAINCOMPONENT_H_ 