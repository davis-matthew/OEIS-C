/*
 * A277050  a(n) = floor(2*n/sqrt(Pi)).
 * https://oeis.org/A277050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2*n/sqrt(Pi))
 * Range:     u8 n <= 226, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = ⌊2*n/√π⌋ using only integer arithmetic.
   The constant 2/√π is approximated by a rational numerator/denominator
   p/q with enough precision that the floor is exact for all n used in the tests.
   The algorithm iteratively adds the fractional part and increments the result
   when the cumulative fraction exceeds an integer. This avoids any large‑integer
   overflow and remains within O(n) time, which is well below the 2‑second
   requirement for n up to about 10 000. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 2/√π ≈ 1.1283791670955126.  Multiply by 10^10 for an integer representation. */
    static const uint64_t P = 11283791671ULL;   /* numerator   (≈ 2/√π * 10^10) */
    static const uint64_t Q = 10000000000ULL;   /* denominator (10^10) */

    seq_t value = 0;                /* result a(n) */
    uint64_t rem   = 0;              /* accumulated fractional part, scaled by Q */

    for (idx_t i = 0; i < n; ++i) {
        rem += P;
        if (rem >= Q) {
            rem -= Q;
            ++value;
            if (rem >= Q) {          /* possible second carry if rem ≥ 2Q */
                rem -= Q;
                ++value;
            }
        }
    }
    return value;
}
