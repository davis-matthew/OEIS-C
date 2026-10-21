/*
 * A255764  Partial sums of A255743.
 * https://oeis.org/A255764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 1279, u64 n <= 9339,
 *            u128 n <= 9339, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partial sums of A255743.
   a(1) = 1
   For n > 1:
     a(n) = a(n-1) + b(n)
   where the term b(n) is defined by
     b(1) = 1,
     b(2) = 9,
     b(2k) = 8 * b(k)          (k ≥ 1),
     b(2k+1) = 9 * 8^{ popcount(2k+1) - 2 }   (k ≥ 0, i.e. a(n) for odd n>1).
   The function uses memoisation; it can be called with indices in any order. */

#include <stddef.h>
#include <stdlib.h>

/* Count the number of set bits in an unsigned 64‑bit integer. */
static inline int popcount64(idx_t x)
{
    int cnt = 0;
    while (x) {
        x &= x - 1;   /* clear the lowest set bit */
        ++cnt;
    }
    return cnt;
}

/* Compute the n‑th term of A255743, the underlying sequence. */
static inline seq_t term_b(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)9;

    if ((n & 1) == 0) {                 /* even n */
        return (seq_t)8 * term_b(n >> 1);
    }

    /* odd n > 1 */
    int pc = popcount64(n);              /* popcount ≥ 2 */
    seq_t val = (seq_t)9;
    for (int i = 0; i < pc - 2; ++i) {   /* multiply by 8^(pc-2) */
        val *= (seq_t)8;
    }
    return val;
}

/* The requested sequence A255764. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for 0, return 0 */

    static seq_t *sum = NULL;   /* cache of partial sums a(i) */
    static size_t  cap  = 0;    /* allocated capacity (indices 0 … cap-1) */
    static size_t  size = 0;    /* highest index computed so far */

    /* Expand the cache if necessary.  We use n+1 slots so that index n is valid. */
    if (n + 1 > cap) {
        size_t new_cap = (size_t)(n + 1);
        if (sum == NULL) {
            sum = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            seq_t *tmp = (seq_t *)realloc(sum, new_cap * sizeof(seq_t));
            if (!tmp) abort();    /* fatal error – should never happen here */
            sum = tmp;
        }
        cap = new_cap;
    }

    /* Initialise the cache if it has not been used yet. */
    if (size == 0) {
        sum[0] = (seq_t)0;              /* unused, just to keep indices aligned */
        sum[1] = term_b(1);             /* equals 1 */
        size = 1;
    }

    /* Compute missing terms up to n. */
    if ((idx_t)n > size) {
        for (idx_t i = size + 1; i <= n; ++i) {
            sum[i] = sum[i - 1] + term_b(i);
        }
        size = (size_t)n;
    }

    return sum[n];
}
