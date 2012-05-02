#include "test_lists.h"

#include "opencv2\core\core.hpp"

#include "cv_helper_lib.h"

namespace OCHL_test{
  namespace color_channel_extractor_test{
    bool ShouldThrowExceptionWhenNotInvokeExtractNewImage(){
      cv_helper::ColorChannelExtractor extractor;
      try {
        extractor.get_channels("R");
      } catch (...) {
        return true;
      }
      return false;
    }
  }
}