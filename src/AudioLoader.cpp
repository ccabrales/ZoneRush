#include "AudioLoader.h"

vector<string> acceptableFileExts {"mp3", "m4a", "wav"};
vector<string> convertFileExts {"aiff", "aif", "flac", "ogg"};


void AudioLoader::start(ofFileDialogResult res) {
    result = res;
    startThread();
}

void AudioLoader::threadedFunction(){
//    ofFileDialogResult res = ofSystemLoadDialog();
    string filePath = result.getPath();
    string ext = ofFilePath::getFileExt(filePath);
    
    if (!result.bSuccess) { //user clicked cancel
        createError("");
        return;
    }
    
    if (ext == "") {
        createError("The selected music piece does not seem to have an extension and thus cannot be read.");
        return;
    }
    
    ofxAudioDecoder* musicDecoder = new ofxAudioDecoder();
    
    //Acceptable format, move along
    if (std::find(acceptableFileExts.begin(), acceptableFileExts.end(), ext)
        != acceptableFileExts.end()) {
        musicDecoder->load(filePath);
        if (musicDecoder->getChannels() != 2 || musicDecoder->getSampleRate() != 44100) {
            
            AudioStreamBasicDescription outputFormat = {0};
            outputFormat.mSampleRate = musicDecoder->getSampleRate();
            outputFormat.mFormatID = kAudioFormatLinearPCM;
            outputFormat.mBitsPerChannel = 32;
            outputFormat.mFormatFlags = kAudioFormatFlagIsFloat | kAudioFormatFlagIsPacked;
            outputFormat.mFramesPerPacket = 1;
            outputFormat.mChannelsPerFrame = 2;
            outputFormat.mBytesPerFrame = 4;
            outputFormat.mBytesPerPacket = 4;

            filePath = "../../../data/temp1.wav";
            AudioFileID outputFile;
            CFStringRef urlStr = CFStringCreateWithCString(kCFAllocatorDefault, filePath.c_str(), kCFStringEncodingUTF8);
            CFURLRef ref = CFURLCreateWithFileSystemPath(NULL, urlStr, kCFURLPOSIXPathStyle, false);
            AudioFileCreateWithURL(ref, kAudioFileWAVEType, &outputFormat, (kAudioFileFlags_EraseFile), &outputFile);
            UInt32 sizeOfBuffer = (UInt32)(musicDecoder->getNumSamples());
            AudioFileWriteBytes(outputFile, FALSE, 0, &sizeOfBuffer, &(musicDecoder->getRawSamples()[0]));
            AudioFileClose(outputFile);
            if(!convertFile(filePath)){
                createError("Attempted to convert music but failed. :( Try again with a different file?");
                return;
            }
            musicDecoder->load("temp.wav");
        }
        
    } else if (std::find(convertFileExts.begin(), convertFileExts.end(), ext)
               != convertFileExts.end()) {
        if(convertFile(filePath)){
            createError("Attempted to convert music but failed. :( Try again with a different file?");
            return;
        }
        musicDecoder->load("temp.wav");
            
    } else {
        createError("Unsupported file type chosen. Please select from the following: mp3, m4a, wav, aiff, aif, flac, ogg");
        return;
    }

    Track* t = new Track(musicDecoder);
    setTrack(t);
    setDecoder(musicDecoder);

    lock();
    hasError = false;
    isDone = true;
    unlock();
}

bool AudioLoader::convertFile(string& filePath) {
    string output = ofSystem("../../../sox "+filePath+" -c 2 -r 44100 -t wav ../../../data/temp.wav && echo \"YES\"");
    //this file will be overwritten every time.
    cout << output << endl;
    
    return (output.find("YES") != std::string::npos);
}

void AudioLoader::createError(string msg){
    lock();
    errorMsg = msg;
    hasError = true;
    isDone = true;
    unlock();
}

void AudioLoader::setTrack(Track* newTrack){
    lock();
    loadedTrack = std::unique_ptr<Track>(newTrack);
    unlock();
}

void AudioLoader::setDecoder(ofxAudioDecoder* newDecoder){
    lock();
    tempDecoder = std::unique_ptr<ofxAudioDecoder>(newDecoder);
    unlock();
}
