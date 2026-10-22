/*
 * A303000  a(n) = permanent of the n X n matrix with entries a(i, i) = i^2
 *          and a(i, j) = 1 elsewhere.
 * https://oeis.org/A303000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = permanent of the n×n matrix with a(i,i)=i^2 and a(i,j)=1
 *            for i≠j
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   deallocate_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/deallocate_seq_array.h"

static seq_t *e = NULL;          /* elementary symmetric sums */
static seq_t *fact = NULL;       /* factorials */
static seq_t *cache = NULL;      /* cached a(n) values */
static idx_t current_max = 0;    /* maximum n processed */
static size_t capacity = 0;      /* allocated size */

/* allocate an array of seq_t, value‑initialized to zero */
static seq_t *allocate_seq_array(size_t len)
{
#if defined(__cplusplus)
    return new seq_t[len]();          /* default‑constructed zeros */
#else
    return (seq_t*)calloc(len, sizeof(seq_t));  /* zeroed memory */
#endif
}

/* ensure storage for indices up to n */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n < capacity) return;   /* already enough */

    size_t new_cap = (size_t)n + 1;      /* storage for indices 0..n */
    seq_t *new_e     = allocate_seq_array(new_cap);
    seq_t *new_fact  = allocate_seq_array(new_cap);
    seq_t *new_cache = allocate_seq_array(new_cap);

    /* copy existing data */
    for (size_t i = 0; i < capacity; ++i) {
        new_e[i]     = e[i];
        new_fact[i]  = fact[i];
        new_cache[i] = cache[i];
    }

    /* free old arrays */
    deallocate_seq_array(e);
    deallocate_seq_array(fact);
    deallocate_seq_array(cache);

    e = new_e;
    fact = new_fact;
    cache = new_cache;
    capacity = new_cap;
}

/* main function computing a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* not used, but safe */

    ensure_capacity(n);

    /* base case for the first computation */
    if (current_max == 0) {
        e[0]     = (seq_t)1;
        fact[0]  = (seq_t)1;
        cache[0] = (seq_t)0;            /* unused, placeholder */
    }

    /* compute values from current_max+1 up to n */
    for (idx_t i = current_max + 1; i <= n; ++i) {
        seq_t ci = (seq_t)i * (seq_t)i - (seq_t)1;   /* c_i = i^2 - 1 */

        /* update elementary symmetric sums e_k */
        /* iterate k from i down to 1 to use previous e[k-1] */
        for (idx_t k = i; k >= 1; --k) {
            e[k] = e[k] + ci * e[k-1];
        }

        /* factorial i! = (i-1)! * i */
        fact[i] = fact[i-1] * (seq_t)i;

        /* compute a(i) = Σ_{k=0}^{i} (i-k)! * e_k */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= i; ++k) {
            sum += fact[i-k] * e[k];
        }
        cache[i] = sum;
    }

    current_max = n;
    return cache[n];
}
