/*
 * A370378  Number of compositions of n where there are (2*k)! sorts of part
 *          k.
 * https://oeis.org/A370378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(n) = Σ_{k=1..n} (2*k)! * a(n-k) for n ≥ 1 (via the
 *            generating function 1/(1-Σ_{k≥1}(2k)!x^k))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *seq_cache = NULL;   /* computed a[i], 0 ≤ i ≤ max */
static idx_t  seq_cache_cap = 0;  /* allocated slots for seq_cache  */
static idx_t  seq_cache_max  = 0; /* largest i that has been filled      */

static seq_t *fact_cache = NULL;  /* factorial[0…] stored up to 2*max */
static idx_t  fact_cache_cap = 0; /* allocated slots for fact_cache     */
static idx_t  fact_cache_max  = 0;/* largest index that has been filled */

/* Ensure fact_cache has hold for indices 0…need.  Reallocate if necessary. */
static inline void ensure_fact(idx_t need)
{
    if (need + 1 <= fact_cache_cap) return;
    size_t newcap = (size_t)(need + 1);
    fact_cache = (seq_t *)realloc(fact_cache, newcap * sizeof(seq_t));
    if (!fact_cache) abort();           /* out of memory */
    fact_cache_cap = newcap;
}

/* Ensure seq_cache has hold for indices 0…need.  Reallocate if necessary. */
static inline void ensure_seq(idx_t need)
{
    if (need + 1 <= seq_cache_cap) return;
    size_t newcap = (size_t)(need + 1);
    seq_cache = (seq_t *)realloc(seq_cache, newcap * sizeof(seq_t));
    if (!seq_cache) abort();           /* out of memory */
    seq_cache_cap = newcap;
}

/* Main entrypoint:  compute the sequence value a(n). */
seq_t a(idx_t n)
{
    /* Handle the offset term a(0) explicitly. */
    if (n == 0) {
        /* Ensure storage for a[0] and set it. */
        if (!seq_cache) {
            ensure_seq(0);
            seq_cache[0] = 1;
            seq_cache_max = 0;
        }
        return seq_cache[0];
    }

    /* Make sure we have factorials up to (2*n)! in fact_cache. */
    ensure_fact(2 * n);
    if (fact_cache_max < 2 * n) {
        /* Create missing factorial values. */
        if (fact_cache_max == 0) {
            fact_cache[0] = 1;       /* 0! = 1 */
            fact_cache_max = 0;
        }
        for (idx_t i = fact_cache_max + 1; i <= 2 * n; ++i) {
            fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
        }
        fact_cache_max = 2 * n;
    }

    /* Compute a[1] … a[n] if they are not yet known. */
    ensure_seq(n);
    if (seq_cache_max < n) {
        /* Make sure a[0] exists. */
        if (seq_cache_max == 0 && seq_cache[0] != 1) {
            seq_cache[0] = 1;
        }
        for (idx_t i = seq_cache_max + 1; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) {
                sum += fact_cache[2 * k] * seq_cache[i - k];
            }
            seq_cache[i] = sum;
        }
        seq_cache_max = n;
    }

    return seq_cache[n];
}
