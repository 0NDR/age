#include "Sound.h"
void Sound::loadFromFile(std::string filepath)
{
    file = filepath;
    SDL_LoadWAV( filepath.c_str(), &AudioSpecification, &data, &length );
    alDeleteBuffers(1,&buffer);
    alBufferData( getBuffer(), getFormat(), data, length, AudioSpecification.freq );
    SDL_FreeWAV( data );
}
ALenum Sound::getFormat()
{
    if(Format != 0)
    {
        return Format;
    }
    if(AudioSpecification.channels == 1)
    {
        if(SDL_AUDIO_BITSIZE(AudioSpecification.format) == 8)
            Format = AL_FORMAT_MONO8;
        else
            Format = AL_FORMAT_MONO16;
    }
    else
    {
        if(SDL_AUDIO_BITSIZE(AudioSpecification.format) == 8)
            Format = AL_FORMAT_STEREO8;
        else
            Format = AL_FORMAT_STEREO16;
    }
    return Format;
}
ALuint Sound::getBuffer()
{
    if(alIsBuffer(buffer)==AL_FALSE)
        alGenBuffers(1,&buffer);
    return buffer;
}

int Sound::getFrequency()
{
    int freq;
    alGetBufferi(getBuffer(),AL_FREQUENCY,&freq);
    return freq;
}
int Sound::getDepthBits()
{
    int db;
    alGetBufferi(getBuffer(),AL_BITS,&db);
    return db;
}
int Sound::getChannels()
{
    int chan;
    alGetBufferi(getBuffer(),AL_CHANNELS,&chan);
    return chan;
}
int Sound::getSize()
{
    int ssize;
    alGetBufferi(getBuffer(),AL_SIZE,&ssize);
    return ssize;
}

std::string Sound::getFilePath()
{
    return file;
}

