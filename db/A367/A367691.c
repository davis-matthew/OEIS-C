/*
 * A367691  G.f. A(x) satisfies A(x) = 1 / ((1 - x) * (1 - x * A(x^3))).
 * https://oeis.org/A367691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 44, u64 n <= 88, u128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0) = 1,
    a(1) = 2, a(2) = 3, a(3) = 4,
    a(n) = 1 + Σ_{k=0}^{⌊(n-1)/3⌋} a(k) * a(n-1-3k)   (n ≥ 1) */

#include <stdlib.h>

static seq_t *a_cache      = NULL; /* computed values */
static size_t a_cache_size = 0;    /* allocated capacity */
static idx_t  a_cache_next = 0;    /* next index to compute */

/* Ensure the cache can hold values up to index n. */
static void a_ensure_cache(idx_t n)
{
    if (a_cache_size > (size_t)n) return;          /* enough space */

    size_t new_cap = a_cache_size ? a_cache_size * 2 : 256;
    if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;

    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0) exit(EXIT_FAILURE); /* allocation failed */
    a_cache      = tmp;
    a_cache_size = new_cap;
}

/* Compute a(n) with memoisation. */
seq_t a(idx_t n)
{
    a_ensure_cache(n);                     /* grow cache if needed */

    for (idx_t i = a_cache_next; i <= n; ++i) {
        if (i == 0) {
            a_cache[i] = (seq_t)1;
        } else {
            seq_t sum = 1;                /* the "+1" term */
            idx_t maxk = (i - 1) / 3;    /* floor((i-1)/3) */
            for (idx_t k = 0; k <= maxk; ++k) {
                sum += a_cache[k] * a_cache[i - 1 - 3 * k];
            }
            a_cache[i] = sum;
        }
    }
    a_cache_next = n + 1;                  /* all indices < n are ready */
    return a_cache[n];
}
