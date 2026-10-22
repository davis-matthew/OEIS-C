/*
 * A325486  One of the four successive approximations up to 5^n for the 5-adic
 *          integer 6^(1/4). This is the 3 (mod 5) case (except for n = 0).
 * https://oeis.org/A325486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the unique integer k in [1,5^n] with k ≡ 3 (mod 5) and
 *            k^4 ≡ 6 (mod 5^n). a(0) is defined to be 0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t, if needed */

/* Multiply two seq_t numbers modulo mod without overflow (works for any
   unsigned seq_t, even very small ones).  Only integer arithmetic and
   bitwise ops allowed. */
static seq_t mulmod(seq_t a, seq_t b, seq_t mod)
{
    seq_t res = 0;
    while (b)
    {
        if (b & 1)
        {
            res += a;
            if (res >= mod)
                res -= mod;
        }
        a <<= 1;      /* a = (a * 2) mod mod */
        if (a >= mod)
            a -= mod;
        b >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* a(0) is defined as 0 */
    if (n == 0)
        return (seq_t)0;

    /* Initial data for the Hensel lift   k ≡ 3 (mod 5),  M = 5 */
    seq_t k = (seq_t)3;   /* current approximation */
    seq_t M = (seq_t)5;   /* modulus 5^current_power */

    /* Perform the lift for each additional 5-adic digit */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t M_next = M * (seq_t)5;            /* 5^(i)   */
        seq_t k2     = mulmod(k, k, M_next);   /* k² mod M_next */
        seq_t k4     = mulmod(k2, k2, M_next); /* k⁴ mod M_next */

        /* diff = (k⁴ - 6) mod M_next in the range [0, M_next) */
        seq_t diff = k4;
        if (diff < (seq_t)6)
            diff += M_next;
        diff -= (seq_t)6;                       /* now diff < M_next */

        /* r = diff / M  (0 ≤ r < 5) */
        seq_t r = diff / M;                     /* integer division */
        idx_t r_int = (idx_t)r;                 /* fits in idx_t */

        /* d ≡ -2·r (mod 5)  (0 ≤ d < 5) */
        idx_t d = (5 - ((2 * r_int) % 5)) % 5;

        /* Update k for the next modulus: k ← k + d·M */
        k = k + M * (seq_t)d;

        /* Advance to the next power of 5 */
        M = M_next;
    }

    return k;
}
