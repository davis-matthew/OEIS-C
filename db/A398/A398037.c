/*
 * A398037  Multiplicative partition persistence of n: number of iterations of
 *          the multiplicative partition function A001055 required to reach 1.
 * https://oeis.org/A398037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n == 1; otherwise a(n) = 1 + a(A001055(n)).
 *            A001055(n) counts unordered factorizations of n into integers >
 *            1. We compute it by a simple recursive divisor enumeration
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

/* Helper: number of unordered factorizations of n with all factors >= min_factor. */
static seq_t count_factorizations(idx_t n, idx_t min_factor)
{
    seq_t total = 1;                     /* the factorization consisting of n itself */
    for (idx_t f = min_factor; f * f <= n; ++f)
    {
        if (n % f == 0)
        {
            idx_t remainder = n / f;
            seq_t sub = count_factorizations(remainder, f);
            total += sub;
        }
    }
    return total;
}

/* Multiplicative partition function A001055: count of unordered factorizations of n. */
static seq_t multiplicative_partitions(idx_t n)
{
    if (n <= 1) return 0;               /* unused for our sequence, but safe */
    return count_factorizations(n, 2);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 1)
        return 0;
    seq_t m = multiplicative_partitions(n);
    return 1 + a((idx_t)m);
}
