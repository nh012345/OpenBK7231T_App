/// @file
/// @brief Support for the Ohm sealing LED lights.
// Supports:
//   Brand: Ohm,  Model: IR blaster.

// #include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

// Constants
const uint16_t kOhmTick = 700;
const uint16_t kOhmHdrMark = 9000;
const uint16_t kOhmHdrSpace = 4500;
const uint16_t kOhmBitMark = 700;
const uint16_t kOhmOneSpace = 2100;
const uint16_t kOhmZeroSpace = kOhmBitMark;
const uint16_t kOhmMinGap = 36000;

#if SEND_OHM
/// Send a Inax Toilet formatted message.
/// Status: STABLE / Working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/706
void IRsend::sendOhm(const uint64_t data, const uint16_t nbits,
                      const uint16_t repeat) {
  sendGeneric(kOhmHdrMark, kOhmHdrSpace,
              kOhmBitMark, kOhmOneSpace,
              kOhmBitMark, kOhmZeroSpace,
              kOhmBitMark, kOhmMinGap,
              data, nbits, 38, true, repeat, kDutyDefault);
}
#endif  // SEND_OHM

#if DECODE_OHM
/// Decode the supplied Ohm sealing LED light IR blaster.
/// Status: Stable / Known working.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/706
bool IRrecv::decodeOhm(decode_results *results, uint16_t offset,
                        const uint16_t nbits, const bool strict) {
  if (strict && nbits != kOhmBits)
    return false;  // We expect Ohm to be a certain sized message.

  uint64_t data = 0;

  // Match Header + Data + Footer
  if (!matchGeneric(results->rawbuf + offset, &data,
                    results->rawlen - offset, nbits,
                    kOhmHdrMark, kOhmHdrSpace,
                    kOhmBitMark, kOhmOneSpace,
                    kOhmBitMark, kOhmZeroSpace,
                    kOhmBitMark, kOhmMinGap, true)) return false;
  // Success
  results->bits = nbits;
  results->value = data;
  results->decode_type = decode_type_t::OHM;
  results->command = 0;
  results->address = 0;
  return true;
}
#endif  // DECODE_OHM
