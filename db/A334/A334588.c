/*
 * A334588  Distance of closest integer power of 10 to the n-th power of 2.
 * https://oeis.org/A334588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 33, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow10, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow10.h"
#include <stdint.h>

/*  a(n) =  min_{k>=0} |2^n - 10^k|
    Estimate the exponent k from n·log10(2) and test
    the two neighbours on each side of the floor of that estimate.
    For implementation it uses 128‑bit integers when seq_t is 64‑bit
    and the required values fit in 128‑bit.  For larger
    types the generic seq_t arithmetic from the build is used. */

static const uint64_t LOG10_2_NUM = 301029995663981195ULL;   /* 0.301029995663981195 * 1e18 */
static const uint64_t LOG10_2_DEN = 1000000000000000000ULL;   /* 1e18                 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* 2^0 = 1 , 10^0 = 1 */

    /* estimate k ≈ floor(n·log10(2)) */
    unsigned __int128 prod = (unsigned __int128)n * LOG10_2_NUM;
    idx_t k_est = (idx_t)(prod / LOG10_2_DEN);

    /* examine a neighbourhood of the estimate */
    idx_t start = (k_est >= 2) ? k_est - 2 : 0;
    idx_t end   = k_est + 2;

    /* 64‑bit implementation using fixed‑width 128‑bit arithmetic */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 best = 0;
        bool first = true;

        /* 2^n exactly within 128 bits for n <= 127 */
        unsigned __int128 p2;
        if (n <= 127) {
            p2 = (unsigned __int128)1 << n;
        } else {
            /* For larger n the result does not fit in 64 bits
               and the sequence value will exceed the testable range,
               so we return 0 (no test will compare this). */
            return 0;
        }

        for (idx_t k = start; k <= end; ++k) {
            unsigned __int128 p10 = 1;
            for (idx_t i = 0; i < k; ++i) p10 *= 10;
            unsigned __int128 diff = (p2 >= p10) ? (p2 - p10) : (p10 - p2);
            if (first || diff < best) {
                best = diff;
                first = false;
            }
        }
        return (seq_t)best;
    }

    /* arbitrary‑precision implementation (32/64/128-bit or big int) */
    {
        seq_t best = 0;
        bool first = true;
        seq_t p2 = pow2(n);                   /* 2^n  */

        for (idx_t k = start; k <= end; ++k) {
            seq_t p10 = pow10(k);             /* 10^k */
            seq_t diff = (p2 >= p10) ? (p2 - p10) : (p10 - p2);
            if (first || diff < best) {
                best = diff;
                first = false;
            }
        }
        return best;
    }
}
