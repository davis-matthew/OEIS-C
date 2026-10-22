/*
 * A396513  a(n) is the minimum integer k such that rad(k)=rad(k+2n), where
 *          rad(x) is the squarefree kernel of x (A007947).
 * https://oeis.org/A396513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force search for the smallest k such that rad(k) == rad(k + 2n) */
#include <stdint.h>
#include "../utils/rad.h"

seq_t a(idx_t n)
{
    /* 2*n is an upper bound for the minimal solution (k = 2n always works). */
    idx_t upper = 2 * n;
    for (idx_t k = 1; k <= upper; ++k)
    {
        if (rad(k) == rad(k + 2 * n))
            return (seq_t)k;
    }
    /* Fallback: the guaranteed solution k = 2n. */
    return (seq_t)upper;
}
