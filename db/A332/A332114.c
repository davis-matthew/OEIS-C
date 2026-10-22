/*
 * A332114  a(n) = (10^(2*n+1) - 1)/9 + 3*10^n.
 * https://oeis.org/A332114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (10^(2*n+1) - 1)/9 + 3*10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

/* 
 * Computes the n-th term of OEIS sequence A332114.
 * a(n) = (10^(2*n+1) - 1)/9 + 3*10^n
 *
 * The function works for any implementation of seq_t (built‑in integer
 * types or big‑integer class) and for any idx_t (here uint64_t).
 */
seq_t a(idx_t n)
{
    /* Compute 10^(2*n+1) and 10^n */
    seq_t pow2n1 = pow10((idx_t)(2 * n + 1));
    seq_t pow_n   = pow10(n);

    /* (10^(2*n+1) - 1)/9  +  3*10^n  */
    seq_t ones = (pow2n1 - 1) / 9;
    seq_t res  = ones + (pow_n * 3);

    return res;
}
