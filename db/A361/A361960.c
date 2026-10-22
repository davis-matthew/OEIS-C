/*
 * A361960  Total semiperimeter of 2-Fuss-Catalan polyominoes of length 2n.
 * https://oeis.org/A361960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*binomial(3*n-2, n) + binomial(3*n-2, n-3)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a multiplicative binomial routine that
   employs 128‑bit temporaries to avoid intermediate overflow when
   seq_t is a 64‑bit unsigned integer.  For larger arbitrary‑precision
   seq_t types the temporary cast is harmless because the compiler
   will perform a conversion to the larger type. */

#include <stdint.h>

static seq_t binom(idx_t n, idx_t k)
{
    /* k chosen as n-k if that reduces the loop */
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* use 128‑bit to hold intermediate product safely */
        unsigned __int128 prod = (unsigned __int128)res *
                                (unsigned __int128)(n - k + i);
        res = (seq_t)(prod / (unsigned __int128)i);
    }
    return res;
}

seq_t a(idx_t n)
{
    /* offset: n starts at 1 */
    seq_t term1 = binom(3 * n - 2, n);
    seq_t term2 = 0;
    if (n >= 3)
        term2 = binom(3 * n - 2, n - 3);
    return (seq_t)2 * term1 + term2;
}
