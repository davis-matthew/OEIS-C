/*
 * A279886  a(n) = A057863(n+1) * Sum_{k=0..n}(k! / (2*k+1)!!).
 * https://oeis.org/A279886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A057863(n+1) * Σ_{k=0..n} k!/(2k+1)!!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (used for implementation)                            */
/*   a(0) = 1                                                      */
/*   a(n) = a(n-1)*(2n+1)!! + n!*A057863(n)  (n≥1)                */
/* Where A057863(1)=1 and A057863(i)=A057863(i-1)*(2i-1)!!           */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Utility for allocating an array of seq_t of arbitrary precision.  */
#ifndef seq_alloc_new_array
#define seq_alloc_new_array(n) ((seq_t *)malloc((n)*sizeof(seq_t)))
#endif

/* Static state to memoize computed a(n) values */
static idx_t  cache_max = 0;      /* highest n for which a(n) is stored */
static size_t cache_cap = 0;      /* allocated capacity of cache */
static seq_t *a_cache  = NULL;    /* cached a(n) values */

static seq_t a_prev  = 0;  /* a(current_max) */
static seq_t fact    = 1;  /* factorial of current_max */
static seq_t A_val   = 1;  /* A057863(current_max) */
static seq_t df_odd  = 1;  /* (2*current_max-1)!!   */

/* Helper to enlarge the cache if needed */
static void ensure_cache(idx_t n)
{
    if (cache_cap > n) return;
    size_t new_cap = (n+1) > 4 ? (n+1)*2 : 4;
    seq_t *new_cache = seq_alloc_new_array(new_cap);
    if (!new_cache) {
        /* Allocation failure – abort (behaviour unspecified) */
        abort();
    }
    /* Copy old contents */
    for (size_t i = 0; i <= cache_max; ++i) {
        new_cache[i] = a_cache[i];
    }
    /* Remaining entries zero-initialized */
    for (size_t i = cache_max+1; i < new_cap; ++i) {
        new_cache[i] = 0;
    }
    if (a_cache) free(a_cache);
    a_cache    = new_cache;
    cache_cap  = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Base case n == 0 */
    if (n == 0) {
        /* Initialise lazy state on first call */
        if (cache_cap == 0) {
            a_prev  = 1;
            fact    = 1;
            A_val   = 1;
            df_odd  = 1;   /* for n=0: (2*0-1)!! = (-1)!! = 1 by convention */
            cache_cap = 4;
            a_cache  = seq_alloc_new_array(cache_cap);
            a_cache[0] = a_prev;
            cache_max = 0;
        }
        return 1;
    }

    /* Ensure cache covers requested n */
    ensure_cache(n);

    /* If already computed, return cached value */
    if (cache_max >= n) return a_cache[n];

    /* Compute values incrementally from (cache_max+1) up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* Odd double factorial for (2*i-1)!!  */
        seq_t df_i = df_odd * (seq_t)(2 * i - 1);          /* (2i-1)!!   */
        /* Update A057863(i) */
        seq_t A_new = A_val * df_i;                       /* A(i)       */
        /* Update factorial i!  */
        seq_t fact_new = fact * (seq_t)i;                /* i!         */
        /* (2i+1)!! = (2i+1) * (2i-1)!! = (2i+1) * df_i */
        seq_t double_fact_next = (seq_t)(2 * i + 1) * df_i;
        /* Compute a(i) using the recurrence */
        seq_t a_new = a_prev * double_fact_next + fact_new * A_new;
        /* Store result */
        a_cache[i] = a_new;
        /* Advance state for next iteration */
        df_odd  = df_i;
        fact    = fact_new;
        A_val   = A_new;
        a_prev  = a_new;
        cache_max = i;
    }

    return a_cache[n];
}
