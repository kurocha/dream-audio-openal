//
//  Audio/Stream.h
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 7/10/09.
//  Copyright (c) 2009 Samuel Williams. All rights reserved.
//
//

#ifndef _DREAM_AUDIO_STREAM_H
#define _DREAM_AUDIO_STREAM_H

#include "Sound.h"
#include <Dream/Events/Loop.h>
#include <set>

namespace Dream
{
	namespace Audio
	{
		using namespace Dream::Resources;
		using namespace Dream::Events;

		const std::size_t ChunkSize = 1024 * 4 * 32;
		const unsigned BufferCount = 3;

		ALenum bytes_per_sample (ALenum format);

		class Stream;

		class IStreamDelegate {
		public:
			virtual ~IStreamDelegate();

			virtual void stream_will_play(Ptr<Stream> stream);
			virtual void stream_did_pause(Ptr<Stream> stream);
			virtual void stream_did_stop(Ptr<Stream> stream);

			virtual void stream_did_queue_buffer(Ptr<Stream> stream, ALenum format, const ALvoid * data, ALsizei size);
		};

		class Stream : public Object, implements IStreamable {
		protected:
			Ptr<IStreamDelegate> _delegate;

			Ref<Source> _source;
			ALenum _format;
			ALsizei _frequency;

			std::vector<ALuint> _buffers;

			Ref<TimerSource> _timer;
			Ref<Fader> _fader;

			void buffer_callback ();
			void start_buffer_callbacks (Ptr<Events::Loop> loop);
			void stop_buffer_callbacks ();

			// We provide additional callbacks for processing data
			virtual void buffer_data(Ptr<Source> source, ALuint buffer, ALenum format, const ALvoid *data, ALsizei size, ALsizei freq);

		public:
			Stream (Ptr<Source> source, ALenum format, ALsizei frequency);
			virtual ~Stream ();

			Ref<Source> source () { return _source; }

			/// Start the audio stream playing.
			/// If the audio stream has been paused, it may resume from that point.
			/// If the stream is real time, it may resume playing audio at the current time.
			virtual void play (Ptr<Events::Loop> loop);

			/// Pause the audio stream. The audio stream may be resumed at this point exactly.
			virtual void pause ();

			/// Stop the audio stream and discard any buffers that have been queued.
			virtual void stop ();

			/// Play the stream and linearly interpolate the current gain to the given.
			/// When the fade is complete, the loop is stopped.
			void fade_out (Ptr<Events::Loop> loop, TimeT duration = 0.1, GainT gain = 0.0);

			/// Play the stream and linearly interpolate the current gain to the given gain.
			void fade_in (Ptr<Events::Loop> loop, TimeT duration = 0.1, GainT gain = 1.0);

			TimeT seconds_per_buffer () const;

			/// Delegate is not retained.
			void set_delegate(Ptr<IStreamDelegate> delegate);
			Ptr<IStreamDelegate> delegate ();
		};
	}
}

#endif
