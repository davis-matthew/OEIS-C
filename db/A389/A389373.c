/*
 * A389373  Expansion of g.f.: x/((1-x^2)^3*(1-2*x)).
 * https://oeis.org/A389373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^(n+9) - (18n^2+168n+406))/432 if n is odd, a(n) =
 *            (2^(n+9) - (36n^2+264n+512))/432 if n is even
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* Use 64‑bit arithmetic for small n to avoid arbitrary‑precision overhead.
       For larger n we fall back to the arbitrary‑precision version provided
       by pow_int, which works correctly for any seq_t that supports usual
       arithmetic operators.  This guarantees exact results for all n whose
       terms fit in the target integer type. */
    if (n + 9ULL < 64ULL) {
        uint64_t tn   = (uint64_t)n;
        uint64_t pow  = 1ULL << (n + 9);
        uint64_t sub  = (n & 1U)
                        ? 18ULL * tn * tn + 168ULL * tn + 406ULL
                        : 36ULL * tn * tn + 264ULL * tn + 512ULL;
        uint64_t num  = pow - sub;
        return (seq_t)(num / 432ULL);
    } else {
        seq_t tn   = (seq_t)n;
        seq_t pow  = pow_int((seq_t)2, n + 9);
        seq_t sub  = (n & 1U)
                    ? (seq_t)18 * tn * tn + (seq_t)168 * tn + (seq_t)406
                    : (seq_t)36 * tn * tn + (seq_t)264 * tn + (seq_t)512;
        seq_t num  = pow - sub;
        return num / (seq_t)432;
    }
}
