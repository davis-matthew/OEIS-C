/*
 * A361011  Decimal expansion of conventional value of ampere-90 (A_{90}).
 * https://oeis.org/A361011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, nonn, fini, full
 * Formula:   a(n) = digit of 1 + 8887143810491801/10^23
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint64_t POW10[16] = {
    1ULL,              // 10^0
   10ULL,              // 10^1
  100ULL,              // 10^2
 1000ULL,              // 10^3
 10000ULL,             // 10^4
100000ULL,             // 10^5
1000000ULL,            // 10^6
10000000ULL,           // 10^7
100000000ULL,          // 10^8
1000000000ULL,         // 10^9
10000000000ULL,        // 10^10
100000000000ULL,       // 10^11
1000000000000ULL,      // 10^12
10000000000000ULL,     // 10^13
100000000000000ULL,    // 10^14
1000000000000000ULL    // 10^15
};

seq_t a(idx_t n)
{
    /* integer part */
    if (n == 1) return (seq_t)1;

    /* n-1 is the position in the decimal part (1‑based) */
    idx_t d = n - 1;

    /* leading zeros (first 7 decimal digits) */
    if (d <= 7) return (seq_t)0;

    /* after the terminating decimal, all zeros */
    if (d > 23) return (seq_t)0;

    /* exponent of 10 needed for this digit (0…15) */
    uint64_t k = 23 - d;
    uint64_t div = POW10[k];

    /* numerator of the fraction 8887143810491801/10^23  */
    const uint64_t NUM = 8887143810491801ULL;

    uint64_t dig = (NUM / div) % 10;
    return (seq_t)dig;
}
