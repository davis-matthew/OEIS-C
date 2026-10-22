/*
 * A347437  Number of factorizations of n with integer alternating product.
 * https://oeis.org/A347437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1808, u16 n <= 9933, u32 n <= 9933, u64 n <= 9933,
 *            u128 n <= 9933, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A347437:
   Number of factorizations of n with integer alternating product.
   A factorization is a weakly increasing sequence of integers >1 with product n.
   The alternating product of (y1,…,yk) is Π yi^((-1)^(i-1)).
   We count factorizations where this alternating product is an integer,
   equivalently where the product of odd‑position factors is divisible by
   the product of even‑position factors.
   Complexity: O(number of factorizations of n) per call, < 2 s for n ≤ 10000. */

#include <stdbool.h>   /* for bool, though not used directly */

static seq_t count_fact(idx_t rem, idx_t minf, idx_t pos, seq_t a, seq_t b)
{
    /* If all factors chosen, check divisibility. */
    if (rem == 1)
        return (a % b == 0) ? 1 : 0;

    seq_t total = 0;
    for (idx_t f = minf; f <= rem; ++f)
    {
        if (rem % f != 0) continue;

        seq_t na = a;
        seq_t nb = b;
        if ((pos & 1U) != 0)   /* odd position (1‑based) */
            na *= (seq_t)f;
        else
            nb *= (seq_t)f;

        total += count_fact(rem / f, f, pos + 1, na, nb);
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n == 1)                /* empty factorization counts as 1 */
        return 1;

    /* Start recursion with first factor >= 2, position 1. */
    return count_fact(n, 2, 1, 1, 1);
}
