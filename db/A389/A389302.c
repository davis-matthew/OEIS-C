/*
 * A389302  Position of records in A389235.
 * https://oeis.org/A389302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1008*10^(n-2) for n>7; a(1..7) =
 *            {2,12,108,1944,52488,1102248,44641044}
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n == 0 we return 0 (out of range). */
    if (n == 0) return (seq_t)0;

    /* First seven terms are exceptional.  Store them in a 64‑bit array to avoid
       overflow of the constants when the build uses a smaller seq_t type. */
    if (n <= 7) {
        static const uint64_t first[7] = {
            2ULL,          /* a(1) */
            12ULL,         /* a(2) */
            108ULL,        /* a(3) */
            1944ULL,       /* a(4) */
            52488ULL,      /* a(5) */
            1102248ULL,    /* a(6) */
            44641044ULL    /* a(7) */
        };
        return (seq_t)first[n - 1];
    }

    /* For n > 7, a(n) = 1008 * 10^(n-2).  Compute the power of ten with the
       provided pow10 helper and multiply.  The multiplication is performed
       in the seq_t domain; any overflow for small seq_t types is harmless
       because those values are never requested in those builds. */
    seq_t pow_val = pow10(n - 2);
    seq_t result  = (seq_t)1008 * pow_val;
    return result;
}
