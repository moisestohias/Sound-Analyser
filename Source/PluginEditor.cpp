/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
SoundAnalyserAudioProcessorEditor::SoundAnalyserAudioProcessorEditor (SoundAnalyserAudioProcessor* ownerFilter,  ValueTree analyserTree_)
    : AudioProcessorEditor (ownerFilter), analyserTree(analyserTree_)
{
    // This is where our plugin's editor size is set.
    setSize (600, 500);
    
    // -------------------------------------------------
    // RMS
    setupAnalysisComponents(&sendRMSButton, &RMSLabel,"Root Mean Square (RMS)");
    
    // -------------------------------------------------
    // PEAK ENERGY
    setupAnalysisComponents(&sendPeakButton, &peakLabel,"Peak Energy");

    // -------------------------------------------------
    // SPECTRAL CENTROID
    setupAnalysisComponents(&sendSpectralCentroidButton, &spectralCentroidLabel, "Spectral Centroid");
    
    
    newAnalysisButton.setButtonText("+");
    addAndMakeVisible(&newAnalysisButton);
    newAnalysisButton.addListener(this);
    
    // LISTENERS
    sendRMSButton.addListener(this);
    sendPeakButton.addListener(this);
    sendSpectralCentroidButton.addListener(this);
    
    
    
    plotHeight = 150;
    plotY = 25;

    analyserTree.addListener(this);
    
    startTimer (50);
}



//==============================================================================
SoundAnalyserAudioProcessorEditor::~SoundAnalyserAudioProcessorEditor()
{
}

//==============================================================================
void SoundAnalyserAudioProcessorEditor::setupAnalysisComponents(TextButton* button,Label* label,String labelText)
{
    label->setText(labelText, dontSendNotification);
    button->setColour(TextButton::ColourIds::buttonOnColourId, Colours::blueviolet);
    button->setColour(TextButton::ColourIds::buttonColourId, Colours::silver);
    button->setToggleState(false, dontSendNotification);

    addAndMakeVisible(button);
    addAndMakeVisible(label);
    
}

//==============================================================================
void SoundAnalyserAudioProcessorEditor::paint (Graphics& g)
{    
    g.setColour (Colours::darkgrey);
    g.fillAll (Colours::purple);
    

    
    int N = getProcessor()->analyser.plotHistory.size();
    
    int plotX = (getWidth()- N)/2;
    
    
    g.fillRect(plotX, plotY, N, plotHeight);
    
    g.setColour(Colours::lightsteelblue);

    float previousValue = getProcessor()->analyser.plotHistory[0];
    
    // get the max value
    float maxValue = -10000;
    for (int i = 0;i < N;i++)
    {
        if (getProcessor()->analyser.plotHistory[i] > maxValue)
        {
            maxValue = getProcessor()->analyser.plotHistory[i];
        }
    }
    
    // do the plotting
    for (int i = 0;i < N-1;i++)
    {
        float currentValue = getProcessor()->analyser.plotHistory[i+1];
        
        int p1 = plotY + (plotHeight - ((previousValue/maxValue)*plotHeight));
        int p2 = plotY + (plotHeight - ((currentValue/maxValue)*plotHeight));
        
        g.drawLine(plotX+i,p1,plotX+i+1,p2);
        previousValue = currentValue;
    }
    
    
    
    
    //g.setFont (15.0f);
    //g.drawFittedText (String(previousValue), 0, 0, getWidth(), getHeight(), Justification::centred, 1);

   // g.drawLine(10, 10, getWidth()-10, getHeight()-10);
}

//==============================================================================
void SoundAnalyserAudioProcessorEditor::resized()
{
    for (int i = 0;i < analysisComponents.size();i++)
    {
        analysisComponents[i]->setBounds(10,185+(i*30),analysisComponents[i]->getWidth(),analysisComponents[i]->getHeight());
    }
    
    /*
    // the vertical position where the lists begin
    int beginListPosY = 185;
    
    // the row height
    int rowHeight = 20;
    
    // the size of the buttons
    int buttonSize = rowHeight;
    
    // the horizontal position of the button
    int buttonPosX = 220;
    
    // the horizontal positon of the label
    int labelPosX = 10;
    
    // the width of the label
    int labelWidth = 200;
    
    
        
    RMSLabel.setBounds(labelPosX,beginListPosY,labelWidth,rowHeight);
    sendRMSButton.setBounds(buttonPosX, beginListPosY, buttonSize, buttonSize);
    
    peakLabel.setBounds(labelPosX, beginListPosY+25, labelWidth, rowHeight);
    sendPeakButton.setBounds(buttonPosX, beginListPosY+25, buttonSize, buttonSize);
    
    spectralCentroidLabel.setBounds(labelPosX, beginListPosY+50, labelWidth, rowHeight);
    sendSpectralCentroidButton.setBounds(buttonPosX, beginListPosY+50, buttonSize, buttonSize);
     */
    newAnalysisButton.setBounds(10, getHeight()-100, 50, 50);
    
    
}

//==============================================================================
void SoundAnalyserAudioProcessorEditor::timerCallback()
{
    float RMSstate_f = getProcessor()->getParameter(getProcessor()->Parameters::pSendRMS);
    bool RMSstate = getProcessor()->floatToBoolean(RMSstate_f);
    sendRMSButton.setToggleState(RMSstate, dontSendNotification);
    
    float peakState_f = getProcessor()->getParameter(getProcessor()->Parameters::pSendPeak);
    bool peakState = getProcessor()->floatToBoolean(peakState_f);
    sendPeakButton.setToggleState(peakState, dontSendNotification);
    
    float specCentState_f = getProcessor()->getParameter(getProcessor()->Parameters::pSendSpectralCentroid);
    bool specCentState = getProcessor()->floatToBoolean(specCentState_f);
    sendSpectralCentroidButton.setToggleState(specCentState, dontSendNotification);
    
    //DBG("timer debug message");
    
    repaint();
}


//==============================================================================
void SoundAnalyserAudioProcessorEditor::buttonClicked (Button* button)
{
    /*
    // -----------------------------------------------
    if (button == &sendRMSButton)
    {
        // get button state
        bool state = sendRMSButton.getToggleState();
        
        // if it is on
        if (state == true)
        {            
            // set the boolean parameter to 0.0
            getProcessor()->setParameterNotifyingHost (SoundAnalyserAudioProcessor::pSendRMS,0.0);
        }
        else // if it is off
        {
            // set the boolean parameter to 1.0
            getProcessor()->setParameterNotifyingHost (SoundAnalyserAudioProcessor::pSendRMS,1.0);
        }
        
        sendRMSButton.setToggleState(!state, dontSendNotification);
   
    }
    
    // -----------------------------------------------
    if (button == &sendPeakButton)
    {
        // get button state
        bool state = sendPeakButton.getToggleState();
        
        // if it is on
        if (state == true)
        {
            // set the boolean parameter to 0.0
            getProcessor()->setParameterNotifyingHost (SoundAnalyserAudioProcessor::pSendPeak,0.0);
        }
        else
        {
            // set the boolean parameter to 1.0
            getProcessor()->setParameterNotifyingHost (SoundAnalyserAudioProcessor::pSendPeak,1.0);
        }
        
        sendPeakButton.setToggleState(!state, dontSendNotification);
        
    }
    
    // -----------------------------------------------
    if (button == &sendSpectralCentroidButton)
    {
        // get button state
        bool state = sendSpectralCentroidButton.getToggleState();
        
        // if it is on
        if (state == true)
        {
            // set the boolean parameter to 0.0
            getProcessor()->setParameterNotifyingHost (SoundAnalyserAudioProcessor::pSendSpectralCentroid,0.0);
        }
        else
        {
            // set the boolean parameter to 1.0
            getProcessor()->setParameterNotifyingHost (SoundAnalyserAudioProcessor::pSendSpectralCentroid,1.0);
        }
        
        sendSpectralCentroidButton.setToggleState(!state, dontSendNotification);
    }*/
    
    if (button == &newAnalysisButton)
    {
        AlertWindow w ("Add new analysis..",
                       "Please slect a new device from the list below",
                       AlertWindow::NoIcon);
        
        StringArray options;
        
        for (int i = 0;i < AnalysisModel::Analyses::NumAnalyses;i++)
        {
            options.add(AnalysisModel::getAnalysisName(i));
        }
        
        w.addComboBox ("option", options, "some options");
        
        w.addButton ("ok",     1, KeyPress (KeyPress::returnKey, 0, 0));
        w.addButton ("cancel", 0, KeyPress (KeyPress::escapeKey, 0, 0));
        
        if (w.runModalLoop() != 0) // is they picked 'ok'
        {
            // this is the item they chose in the drop-down list..
            const int optionIndexChosen = w.getComboBoxComponent ("option")->getSelectedItemIndex();
            
            
            
            AnalysisModel::addNewAnalysis(analyserTree,optionIndexChosen);
            
            /*
            switch (optionIndexChosen)
            {
                case AnalysisModel::RMS:
                    
                    analyserTree.addChild(<#const juce::ValueTree &child#>, <#int index#>, <#juce::UndoManager *undoManager#>)
                    break;
                    
                default:
                    break;
            }*/
            
        }
    }
}

void SoundAnalyserAudioProcessorEditor::addAnalysis(ValueTree& analysisTree)
{
    if (analysisTree.getType() == AnalysisModel::AnalysisTypes::RMS)
    {
        analysisComponents.add(new RMSComponent(analysisTree));
    }
    
    addChildComponent(analysisComponents.getLast());
    analysisComponents.getLast()->setVisible(true);
    
    resized();
}


void SoundAnalyserAudioProcessorEditor::valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property)
{
   
}

void SoundAnalyserAudioProcessorEditor::valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded)
{
    addAnalysis(childWhichHasBeenAdded);
}

void SoundAnalyserAudioProcessorEditor::valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved)
{
    
}

void SoundAnalyserAudioProcessorEditor::valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved)
{
    
}

void SoundAnalyserAudioProcessorEditor::valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged)
{
    
}
