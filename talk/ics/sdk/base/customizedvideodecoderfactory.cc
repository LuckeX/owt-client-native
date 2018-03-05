/*
 * Intel License
 */

#include "talk/ics/sdk/base/customizedvideodecoderfactory.h"
#include "ics/base/globalconfiguration.h"

CustomizedVideoDecoderFactory::CustomizedVideoDecoderFactory(std::unique_ptr<ics::base::VideoDecoderInterface> external_decoder)
  : external_decoder_(std::move(external_decoder)) {
  supported_codec_types_.clear();

  // Currently, customized H264 and VP8 decoders are supported
  supported_codec_types_.push_back(webrtc::kVideoCodecH264);
  supported_codec_types_.push_back(webrtc::kVideoCodecVP8);
}

CustomizedVideoDecoderFactory::~CustomizedVideoDecoderFactory() {}

webrtc::VideoDecoder* CustomizedVideoDecoderFactory::CreateVideoDecoder(webrtc::VideoCodecType type) {
  if (supported_codec_types_.empty()) {
    return nullptr;
  }

  for (std::vector<webrtc::VideoCodecType>::const_iterator it =
    supported_codec_types_.begin(); it != supported_codec_types_.end();
    ++it) {
      if (*it == type) {
        return new ics::base::CustomizedVideoDecoderProxy(type, external_decoder_->Copy());
      }
  }

  return nullptr;
}

void CustomizedVideoDecoderFactory::DestroyVideoDecoder(
    webrtc::VideoDecoder* decoder) {
    delete decoder;
}