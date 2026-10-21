/*
 * A294062  Sum of the differences of the larger and smaller parts in the
 *          partitions of 2n into two parts with the smaller part squarefree.
 * https://oeis.org/A294062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * ∑_{i=1..n} (n-i) * mu(i)^2, where mu(i)^2 = 1 if i is
 *            square‑free, else 0. The implementation keeps a cached prefix
 *            table of square‑free counts and sums, extending it as needed
 * Range:     u8 n <= 19, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Static prefix tables */
static seq_t *sqfree_cnt = NULL;   /* number of square‑free ≤ i */
static seq_t *sqfree_sum = NULL;   /* sum of square‑free ≤ i */
static size_t cached_cap = 0;      /* table length (≥ 1) */

/* Integer square root (floor) */
static idx_t int_sqrt(idx_t x)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= x) ++r;
    return r;
}

/* Recompute prefix tables up to new_cap (exclusive) */
static void recompute_cache(idx_t new_cap)
{
    /* Allocate temporary flag arrays */
    unsigned char *is_prime = (unsigned char*)malloc(new_cap);
    unsigned char *is_sqfree = (unsigned char*)malloc(new_cap);
    if (!is_prime || !is_sqfree) { /* failure: abort */
        free(is_prime); free(is_sqfree);
        exit(1);
    }

    /* Initialise prime marker */
    memset(is_prime, 1, new_cap);
    if (new_cap > 0) is_prime[0] = 0;
    if (new_cap > 1) is_prime[1] = 0;

    /* Simple Eratosthenes sieve for primes */
    idx_t lim = int_sqrt(new_cap - 1);
    for (idx_t p = 2; p <= lim; ++p)
        if (is_prime[p])
            for (idx_t j = p * p; j < new_cap; j += p)
                is_prime[j] = 0;

    /* Initially mark all numbers ≥1 as square‑free */
    memset(is_sqfree, 1, new_cap);
    if (new_cap > 0) is_sqfree[0] = 0;   /* 0 is not considered */

    /* Remove numbers divisible by a square of a prime */
    for (idx_t p = 2; p < new_cap; ++p)
        if (is_prime[p]) {
            idx_t sq = p * p;
            for (idx_t j = sq; j < new_cap; j += sq)
                is_sqfree[j] = 0;
        }

    /* Allocate new prefix tables */
    seq_t *cnt = (seq_t*)malloc(new_cap * sizeof(seq_t));
    seq_t *sum = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!cnt || !sum) {   /* failure: abort */
        free(is_prime); free(is_sqfree);
        free(cnt); free(sum);
        exit(1);
    }

    /* Prefix computation */
    cnt[0] = 0;
    sum[0] = 0;
    for (idx_t i = 1; i < new_cap; ++i) {
        seq_t f = (seq_t)is_sqfree[i];
        cnt[i] = cnt[i-1] + f;
        sum[i] = sum[i-1] + (seq_t)i * f;
    }

    /* Free temporary arrays */
    free(is_prime);
    free(is_sqfree);

    /* Free old tables if they exist */
    if (sqfree_cnt) free(sqfree_cnt);
    if (sqfree_sum) free(sqfree_sum);

    /* Store new tables */
    sqfree_cnt = cnt;
    sqfree_sum = sum;
    cached_cap = new_cap;
}

/* Ensure cache covers at least indices up to n */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cached_cap) return;
    /* Start with a reasonable size if not yet allocated */
    size_t new_cap = cached_cap ? cached_cap : 64;
    while ((size_t)n >= new_cap) new_cap <<= 1;
    recompute_cache((idx_t)new_cap);
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* unused but defined for safety */
    ensure_cache(n);
    seq_t cnt = sqfree_cnt[n];
    seq_t sum = sqfree_sum[n];
    seq_t term = (seq_t)n * cnt - sum;     /* n*A013928(n+1) - A066779(n) */
    return term * 2;
}
