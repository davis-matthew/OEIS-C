/*
 * A307148  Number of binary partitions of n in which exactly one of the
 *          powers of 2 is used an odd number of times.
 * https://oeis.org/A307148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n is odd) ? p((n-1)/2) : Σ_{i≥1, 2^i≤n} p((n-2^i)/2)
 *            where p(k) = number of binary partitions of k. The routine
 *            builds p(k) on demand using the classical dynamic programming on
 *            powers of two
 * Range:     u8 n <= 49, u16 n <= 223, u32 n <= 1687, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *p = NULL;      /* array of binary partition counts p[0 … N] */
static size_t psize = 0;     /* current allocated size */

/* Recompute the array p[0 … limit] from scratch. */
static void recompute_p(idx_t limit)
{
    size_t sz = (size_t)limit + 1;
    seq_t *tmp = (seq_t *)realloc(p, sz * sizeof(seq_t));
    if (!tmp) abort();                /* out of memory */
    p = tmp;
    for (size_t i = 0; i < sz; ++i) p[i] = 0;
    p[0] = 1;                         /* empty partition */

    /* Add contributions of each power of two. */
    for (idx_t power = 1; power <= limit; power <<= 1) {
        for (idx_t s = power; s <= limit; ++s) {
            p[s] = p[s] + p[s - power];
        }
    }
    psize = sz;
}

/* Ensure that p[0 … limit] exist. */
static void ensure_p(idx_t limit)
{
    if ((size_t)limit + 1 <= psize) return;
    recompute_p(limit);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* a(0) = 0 by definition */

    idx_t half = n >> 1;                  /* floor(n/2) */
    ensure_p(half);

    if (n & 1) {                          /* n is odd */
        return p[(n - 1) >> 1];
    } else {                              /* n is even, n >= 2 */
        seq_t sum = 0;
        for (idx_t power = 2; power <= n; power <<= 1) {
            idx_t idx = (n - power) >> 1; /* (n - 2^i)/2 */
            sum = sum + p[idx];
        }
        return sum;
    }
}
