/*
 * A301483  a(n) = floor(a(n-1)/(2^(1/3)-1)) with a(1)=1.
 * https://oeis.org/A301483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = floor(a(n-1)/(2^(1/3)-1)), a(1) = 1
 * Recurrence for n>4: a(n) = 4*a(n-1) - 2*a(n-3) - a(n-4)   */

/* Standard headers for dynamic allocation */
#include <stdlib.h>

/* static storage for memoised values, 1‑based indexing */
static seq_t *cache = NULL;   /* cache[1..N] holds computed values */
static idx_t cached_max = 0;  /* largest index already computed */
static size_t capacity = 0;   /* number of allocated slots, ≥ cached_max+1 */

/* Ensure cache can store at least indices up to 'need' (inclusive). */
static void ensure_capacity(idx_t need)
{
    if (capacity > (size_t)need) return;          /* already enough space */

    size_t newcap = (size_t)need + 1;           /* +1 for 1‑based indexing */
    seq_t *tmp = (cache == NULL)
                ? (seq_t *)malloc(newcap * sizeof(seq_t))
                : (seq_t *)realloc(cache, newcap * sizeof(seq_t));

    if (tmp == NULL) abort();   /* unrecoverable */

    /* initialise any newly allocated slots to zero */
    for (size_t i = capacity; i < newcap; ++i) tmp[i] = 0;

    cache = tmp;
    capacity = newcap;
}

/* Compute all terms up to the requested index. */
static void compute_upto(idx_t n)
{
    if (cached_max >= n) return;          /* already computed */

    ensure_capacity(n);

    for (idx_t i = cached_max + 1; i <= n; ++i) {
        if (i == 1) { cache[i] = 1; }
        else if (i == 2) { cache[i] = 3; }
        else if (i == 3) { cache[i] = 11; }
        else if (i == 4) { cache[i] = 42; }
        else {
            /* Compute 4*a(i-1)   using repeated additions to avoid
               potential overflow in 4*a(i-1) when seq_t is a small
               unsigned integer type. */
            seq_t v1 = cache[i-1];
            v1 += cache[i-1];
            v1 += cache[i-1];
            v1 += cache[i-1]; /* 4*a(i-1) */

            /* Compute 2*a(i-3) */
            seq_t v2 = cache[i-3];
            v2 += cache[i-3];   /* 2*a(i-3) */

            /* Recurrence: a(i) = 4*a(i-1) - 2*a(i-3) - a(i-4) */
            cache[i] = v1 - v2 - cache[i-4];
        }
    }
    cached_max = n;
}

/* Public function required by the judge. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n=0, return 0 safely */
    compute_upto(n);
    return cache[n];
}
