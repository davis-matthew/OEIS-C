/*
 * A373233  Relative of Hofstadter Q-sequence: a(n) = 0 for n <= 0, a(n) = n
 *          for 1 <= n <= 13; a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3))
 *          for n > 13.
 * https://oeis.org/A373233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n for 1 <= n <= 13; for n > 13, a(n) = a(n - a(n-1)) +
 *            a(n - a(n-2)) + a(n - a(n-3))
 * Range:     u8 n <= 250, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for already computed values.  The 0th element is kept as 0
   to simplify handling of a(n) for n <= 0. */
static seq_t *a_cache   = NULL;   /* Array of a(i).  index 0 is unused (value 0). */
static idx_t   a_cap    = 0;       /* Allocated capacity of the array, including index 0. */
static idx_t   a_computed = 0;    /* Largest index for which a(i) is known. */

/* Grow the cache so it can hold values up to at least idx. */
static void ensure_cache_size(idx_t idx)
{
    if (a_cap > idx) return;               /* already large enough */

    idx_t new_cap = a_cap ? a_cap : 1;
    while (new_cap <= idx) new_cap <<= 1;

    seq_t *new_cache = (seq_t*)realloc(a_cache, (new_cap + 1) * sizeof(seq_t));
    if (!new_cache) {                      /* allocation failed */
        /* In a real project we might handle this more gracefully. */
        exit(1);
    }
    a_cache = new_cache;
    /* Set the 0th element to 0 (everything else is already set). */
    a_cache[0] = 0;
    a_cap = new_cap;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* a(0) = 0, though n is expected >=1. */

    ensure_cache_size(n);

    /* Return immediately if already computed. */
    if (a_computed >= n)
        return a_cache[n];

    /* Compute all missing values up to n. */
    for (idx_t i = a_computed + 1; i <= n; ++i) {
        if (i <= 13) {                     /* Known initial values. */
            a_cache[i] = (seq_t)i;
        } else {
            /* Retrieve a(i-1), a(i-2), a(i-3).  These values are already known. */
            seq_t im1 = a_cache[i - 1];
            seq_t im2 = a_cache[i - 2];
            seq_t im3 = a_cache[i - 3];

            /* Compute the three argument positions with under‑flow protection. */
            idx_t pos1 = 0;
            idx_t pos2 = 0;
            idx_t pos3 = 0;

            if (im1 <= (seq_t)i)          /* a(i-1) <= i guarantees i-a(i-1) >= 0 */
                pos1 = i - (idx_t)im1;
            if (im2 <= (seq_t)i)
                pos2 = i - (idx_t)im2;
            if (im3 <= (seq_t)i)
                pos3 = i - (idx_t)im3;

            /* Add the corresponding a(pos) values (0 if pos == 0). */
            seq_t sum = 0;
            if (pos1 != 0) sum += a_cache[pos1];
            if (pos2 != 0) sum += a_cache[pos2];
            if (pos3 != 0) sum += a_cache[pos3];

            a_cache[i] = sum;
        }
    }

    a_computed = n;
    return a_cache[n];
}
