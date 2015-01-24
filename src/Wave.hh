#pragma once

#include <tuple>
#include <vector>
#include <utility>
#include <functional>


#define NUM_OF_WAVES 4

struct WaveParams
{
    float speed;
    float amplitude;
    float waveLength;
    float steepness;

};

struct WaveDirection
{
    float x;

    float z;
};

struct Wave
{
    Wave(float speed, float amplitude, float waveLength, float dX, float dY, float dZ)
        {
            wp.speed = speed;
            wp.amplitude = amplitude;
            wp.waveLength = waveLength;
            wp.steepness = 0.5;//overtallSteepness / (waveLength * amplitude *  static_cast<float>(NUM_OF_WAVES));
            wd.x = dX;

            wd.z = dZ;
        };
    WaveParams wp;
    WaveDirection wd;
    float waterPlaneLength = 128.0f;
    float overtallSteepness = 0.2f;
};