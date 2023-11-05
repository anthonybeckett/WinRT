// main.cpp
#include "pch.h"
#include "./libs/json/single_include/nlohmann/json.hpp"

using namespace winrt;
using namespace Windows::Media::Playback;
using namespace Windows::Foundation;
using namespace Windows::Media::Control;

int getSecondsFromTimeSpan(TimeSpan timeInput)
{
    std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds>(timeInput);

    return static_cast<int>(seconds.count());
}

int main()
{
    nlohmann::json data;

    init_apartment();

    GlobalSystemMediaTransportControlsSessionManager sessionManager = GlobalSystemMediaTransportControlsSessionManager::RequestAsync().get();

    if (sessionManager)
    {
        GlobalSystemMediaTransportControlsSession session = sessionManager.GetCurrentSession();

        if (session)
        {
            auto mediaProperties = session.TryGetMediaPropertiesAsync().get();

            if (mediaProperties)
            {
                hstring title = mediaProperties.Title();
                hstring artist = mediaProperties.Artist();
                hstring album = mediaProperties.AlbumTitle();

                data["title"] = title.c_str();
                data["artist"] = artist.c_str();
                data["album"] = album.c_str();
            }


        	GlobalSystemMediaTransportControlsSessionPlaybackInfo playbackInfo = session.GetPlaybackInfo();

            if (playbackInfo.PlaybackStatus() == GlobalSystemMediaTransportControlsSessionPlaybackStatus::Playing)
            {
                GlobalSystemMediaTransportControlsSessionTimelineProperties timelineInfo = session.GetTimelineProperties();

                // Get the playback position
                TimeSpan currentPosition = timelineInfo.Position();
                TimeSpan trackLength = timelineInfo.MaxSeekTime();

                data["currentPosition"] = getSecondsFromTimeSpan(currentPosition);
                data["trackLength"] = getSecondsFromTimeSpan(trackLength);
            }
        }
    }

    std::string jsonString = data.dump();

    std::cout << jsonString << std::endl;

    return 0;
}





