/*
 * A272603  Number of permutations of [n] whose cycle lengths are factorials.
 * https://oeis.org/A272603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{f≤n, f = k! for k≥1} (n-1)!/(n-f)! * a(n-f), with
 *            a(0) = 1
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies are provided by the build system. */
#include <stdlib.h>
#include <stdint.h>

/* ---------- Helper: dynamic array for seq_t values ---------- */
static void grow_seq_array(seq_t **p, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = need * 2;
        seq_t *tmp = (seq_t *)realloc(*p, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        *p     = tmp;
        *cap   = new_cap;
    }
}

/* ---------- Helper: dynamic array for idx_t values ---------- */
static void grow_idx_array(idx_t **p, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = need * 2;
        idx_t *tmp = (idx_t *)realloc(*p, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        *p     = tmp;
        *cap   = new_cap;
    }
}

/* ---------- Cached factorials (stored as idx_t) ---------- */
static idx_t *fact_cache = NULL;  /* array of factorial values 1!, 2!, … */
static size_t fact_len   = 0;     /* number of stored factorials      */
static size_t fact_cap   = 0;     /* allocated capacity               */

/* Initialise factorial cache once (up to uint64_t overflow). */
static void init_factorials(void)
{
    if (fact_len != 0) return;          /* already initialised */

    /* grow to hold at least one element (1!) */
    grow_idx_array(&fact_cache, &fact_cap, 1);
    fact_cache[0] = (idx_t)1;           /* 1! */
    fact_len = 1;

    /* generate following factorials until overflow */
    while (1) {
        idx_t last   = fact_cache[fact_len - 1];
        idx_t next_fac = fact_len + 1;                /* (k+1)! from k! */
        if (last > UINT64_MAX / next_fac) break;      /* would overflow */
        idx_t next = last * next_fac;
        grow_idx_array(&fact_cache, &fact_cap, fact_len + 1);
        fact_cache[fact_len] = next;
        fact_len++;
    }
}

/* ---------- Cached sequence values a(n) ---------- */
static seq_t *a_cache = NULL;  /* array holding a[0..] */
static size_t a_cap   = 0;     /* allocated capacity     */
static size_t a_len   = 0;     /* number of values stored */

/* Compute a(n) for any n ≥ 0.  The values are memoised so that
   calls may occur in any order. */
seq_t a(idx_t n)
{
    /* Ensure lazy initialisation of factorial cache */
    init_factorials();

    /* Grow cache to hold up to a[n] */
    grow_seq_array(&a_cache, &a_cap, (size_t)(n + 1));

    /* Base case */
    if (a_len == 0) {
        a_cache[0] = (seq_t)1;    /* a(0) = 1 */
        a_len = 1;
    }

    /* If already computed, return it */
    if (n < a_len) return a_cache[n];

    /* Compute missing a values up to n */
    for (idx_t i = a_len; i <= n; ++i) {
        seq_t sum = (seq_t)0;

        /* Iterate over factorials f = k! ≤ i */
        for (size_t j = 0; j < fact_len && fact_cache[j] <= i; ++j) {
            idx_t f = fact_cache[j];
            /* Compute product = (i-1)! / (i-f)! = Π_{t=1}^{f-1} (i - t)   */
            seq_t prod = (seq_t)1;
            for (idx_t t = 1; t < f; ++t) {
                prod *= (seq_t)(i - t);
            }
            sum += prod * a_cache[i - f];
        }
        a_cache[i] = sum;
    }

    a_len = n + 1;
    return a_cache[n];
}
