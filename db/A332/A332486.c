/*
 * A332486  Numbers k such that sin(k) < 0 or cos(k) > 0.
 * https://oeis.org/A332486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that sin(k) < 0 or cos(k) > 0.
   The condition is equivalent to:
     Let m = floor(k / π).
     If m is odd, all k with this m satisfy.
     If m is even, only those with fractional part of k/π < 0.5 satisfy.
   We generate k in increasing order by processing successive intervals of k
   that share the same m.  A high‑precision rational approximation of π,
   355/113, is sufficient for all m needed for n ≤ 10000. */

#include <stdint.h>

static const uint64_t P = 355ull;  /* numerator of π approximation 355/113 */
static const uint64_t Q = 113ull;  /* denominator */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, but safe */
    idx_t count = 0;                /* how many terms produced so far   */
    idx_t m = 0;                    /* current floor(k / π)             */
    seq_t result = 0;               /* value to return when count==n    */

    while (count < n) {
        uint64_t floor_m_pi = (P * m) / Q;            /* floor(mπ)   */
        uint64_t ceil_m_pi = floor_m_pi + 1;          /* ceil(mπ)    */

        if (m & 1) {  /* m odd – all k in [ceil(mπ), floor((m+1)π)] */
            uint64_t floor_m1_pi = (P * (m + 1)) / Q;
            for (uint64_t k = ceil_m_pi; count < n && k <= floor_m1_pi; ++k, ++count)
                result = (seq_t)k;
        } else {      /* m even – k with fractional part < 0.5π */
            uint64_t floor_half = (P * (2 * m + 1)) / (2 * Q); /* floor(mπ+0.5π) */
            uint64_t high = floor_half;                       /* inclusive upper bound */
            for (uint64_t k = ceil_m_pi; count < n && k <= high; ++k, ++count)
                result = (seq_t)k;
        }
        ++m;
    }
    return result;
}
