
#
#  This file is part of the "Teapot" project, and is released under the MIT license.
#

teapot_version "0.8.0"

define_target "dream-audio-openal" do |target|
	target.build do |environment|
		build_directory(package.path, 'source', environment)
	end
	
	target.depends "Library/Dream"
	target.depends "Library/vorbis"
	target.depends "Library/OpenAL"
	
	target.provides "Library/Dream/Audio" => "Library/Dream/AudioOpenAL"
	
	target.provides "Library/Dream/AudioOpenAL" do
		append linkflags ["-lDreamAudioOpenAL"]
	end
end

define_configuration "dream-audio-openal" do |configuration|
	configuration.public!
	
	configuration.require "vorbis"
end