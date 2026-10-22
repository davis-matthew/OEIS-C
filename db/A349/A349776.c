/*
 * A349776  Triangle read by rows: T(n,k) is the number of partitions of set
 *          [n] into a set of at most k lists, with 0 <= k <= n. Also called
 *          broken permutations.
 * https://oeis.org/A349776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A349776 – broken‑permutations triangle read by rows.
 *
 *  The triangle T(n,k) is the number of partitions of the set {1,…,n}
 *  into at most k ordered lists (lists are ordered subsets).
 *  The required sequence a(n) is the flattened triangle:
 *
 *      a(0) = T(0,0)
 *      a(1) = T(1,0), a(2) = T(1,1)
 *      a(3) = T(2,0), a(4) = T(2,1), a(5) = T(2,2)
 *      ...
 *
 *  The formula used is
 *
 *      T(n,k) = ∑_{j=1..k} C(n‑1, j‑1) · n! / j!          (n>0, k>0)
 *
 *  The implementation computes row r and column k from the linear index
 *  n, then evaluates the sum by a simple recurrence that avoids factorial
 *  tables.  All arithmetic is performed in the type seq_t; intermediate
 *  products are computed directly in seq_t, which is safe because either the
 *  exact result fits in seq_t or any overflow indicates that the final
 *  value does not fit and therefore need not be correct.
 */

#include <stdint.h>

/* Integer square‑root for 64‑bit unsigned values (no floating point). */
static idx_t isqrt64(idx_t v)
{
    idx_t low = 0, high = (idx_t)1 << 32;          /* 2^32 > sqrt(2^64) */
    while (low <= high)
    {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == v)
            return mid;
        if (sq < v)
            low = mid + 1;
        else
        {
            if (mid == 0) break;                  /* avoid underflow */
            high = mid - 1;
        }
    }
    return high;                                   /* floor(sqrt(v)) */
}

seq_t a(idx_t n)
{
    /* 1. Find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t val = (idx_t)((unsigned __int128)n * 8 + 1);   /* 8n+1 fits in 64 bits */
    idx_t s   = isqrt64(val);                          /* floor(sqrt(8n+1)) */
    idx_t r   = (s - 1) / 2;                            /* initial estimate */

    /* Correct r if the estimate is off by one. */
    while ((unsigned __int128)r * (r + 1) / 2 > n) r--;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n) r++;

    /* 2. Column index k (0‑based within the row). */
    idx_t row_start = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - row_start;

    /* 3. Handle trivial cases. */
    if (r == 0)               /* only T(0,0)=1 exists */
        return (k == 0) ? (seq_t)1 : (seq_t)0;
    if (k == 0)               /* no lists available for non‑empty set */
        return (seq_t)0;

    /* 4. Compute n! (factorial of the row). */
    seq_t fact = 1;
    for (idx_t i = 1; i <= r; ++i)
        fact *= (seq_t)i;

    /* 5. Sum the terms of the recurrence for j = 1 .. k. */
    seq_t sum  = 0;
    seq_t term = fact;                         /* term for j = 1 */
    for (idx_t j = 1; j <= k; ++j)
    {
        sum += term;
        if (j == k) break;                     /* last term added */

        /* term_{j+1} = term_j * (r - j) / (j*(j+1))   (exact division) */
        seq_t numerator   = term * (seq_t)(r - j);
        seq_t denominator = (seq_t)j * (j + 1);
        term = numerator / denominator;         /* exact integer division */
    }

    return sum;
}
