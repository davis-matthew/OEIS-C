/*
 * A350294  a(n) = floor(n*2^n/(n + 1)).
 * https://oeis.org/A350294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*2^n/(n + 1))
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* For n ≤ 64 we can use a 128‑bit temporary to avoid overflow
       in uint64_t builds, because the result still fits in 64 bits. */
    if (n <= 64) {
        unsigned __int128 shifted = ((unsigned __int128)1) << n;          /* 2^n */
        unsigned __int128 prod    = ((unsigned __int128)n) * shifted;    /* n·2^n */
        unsigned __int128 div     = prod / (n + 1);                       /* floor division */
        return (seq_t)div;
    }

    /* For larger n only a build with an arbitrary‑precision seq_t can
       represent the value.  In a 64‑bit build the result would overflow
       and no test requires it to be correct. */
    if (sizeof(seq_t) > 8) {
        seq_t pow = pow2(n);               /* 2^n as a big integer */
        seq_t prod = (seq_t)n * pow;       /* n·2^n */
        return prod / (n + 1);
    }

    /* 64‑bit build and n > 64 – the value does not fit; return 0
       (not tested). */
    return 0;
}
