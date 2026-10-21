/*
 * A248369  Least positive integer m such that prime(m+n) - prime(m) divides
 *          m.
 * https://oeis.org/A248369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer m such that prime(m+n) - prime(m)
 *            divides m
 * Range:     u8 n <= 18, u16 n <= 974, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;   /* special case */

    /* For n > 1, a(n) is even and at least 2*n */
    idx_t m = 2 * n;

    for (;; m += 2)   /* iterate over even m */
    {
        seq_t p_m   = (seq_t)get_prime(m);
        seq_t p_mn  = (seq_t)get_prime(m + n);
        seq_t diff  = p_mn - p_m;

        /* diff should never be zero for distinct indices */
        if (diff != 0 && ((seq_t)m % diff) == 0)
            return (seq_t)m;
    }
}
