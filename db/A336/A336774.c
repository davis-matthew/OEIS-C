/*
 * A336774  a(n) is the least number of repetitions such that the result of
 *          the repeated execution of the multiplication f <- f*n started at
 *          f=1 differs from the exact value n^a(n), when the multiplication
 *          is performed using 32-bit single precision floats according to the
 *          IEEE 754 standard.
 * https://oeis.org/A336774
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, fini
 * Formula:   not stated
 * Range:     u8 n <= 9294, u16 n <= 9294, u32 n <= 9294, u64 n <= 9294,
 *            u128 n <= 9294, big any n
 * Helpers:   bitlen_seq_t, float_exact_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes a(n) for sequence A336774: the minimal number of repetitions k
   such that performing f <- f*n starting from f=1 using IEEE 754
   single‑precision floats departs from the exact integer n^k.
   The algorithm iteratively multiplies the floating‑point value and the
   exact integer (stored in an unsigned __int128), checks for overflow or
   loss of exactness, and returns the first k where a difference occurs.
*/
#include <stdbool.h>
#include <math.h>
#include <float.h>
#include "../utils/bitlen_seq_t.h"
#include "../utils/float_exact_uint128.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;                    /* undefined for n < 2 */

    float nf = (float)n;                    /* n as IEEE‑754 32‑bit float */
    float f  = 1.0f;                        /* floating‑point product */

    unsigned __int128 pow_exact = 1;        /* exact integer n^k   */
    const unsigned long long n_ll = (unsigned long long)n;

    for (idx_t k = 1; ; ++k) {
        f = f * nf;                          /* multiply in float */

        pow_exact = pow_exact * (unsigned __int128)n_ll; /* exact multiplication */

        if (isinf((double)f))                /* overflow in floating‑point */
            return (seq_t)k;

        if (!float_exact_uint128(pow_exact)) /* n^k cannot be represented
                                               exactly as a binary32 float   */
            return (seq_t)k;
    }
}
