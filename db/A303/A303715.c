/*
 * A303715  Number of nX4 0..1 arrays with every element unequal to 0, 1 or 5
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 3*a(n-3) + a(n-4) - a(n-5) - a(n-6)
 *            Initial values: a(1) = 5, a(2) = 5, a(3) = 8, a(4) = 17, a(5) =
 *            36, a(6) = 76 The function memoises computed values and expands
 *            the cache as needed
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 58, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *seq_cache = NULL;  /* buffer holding a(1)..a(n)  (0‑based index) */
static idx_t   seq_size = 0;     /* number of values currently stored  */
static idx_t   seq_cap  = 0;     /* allocated capacity of seq_cache */

/* Ensure the cache can hold at least 'need' values; expand if necessary. */
static void ensure_seq_capacity(idx_t need)
{
    if (need <= seq_cap) return;

    idx_t new_cap = seq_cap ? seq_cap : 8;
    while (new_cap < need) new_cap <<= 1;

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) abort();            /* memory allocation failed */

    if (seq_cache) {
        memcpy(new_cache, seq_cache, seq_size * sizeof(seq_t));
        free(seq_cache);
    }
    seq_cache = new_cache;
    seq_cap  = new_cap;
}

/* If the cache has not yet been initialised, fill it with the first six terms. */
static void init_seq_cache(void)
{
    if (seq_cache) return;            /* already initialised */

    ensure_seq_capacity(6);
    seq_cache[0] = 5;  /* a(1) */
    seq_cache[1] = 5;  /* a(2) */
    seq_cache[2] = 8;  /* a(3) */
    seq_cache[3] = 17; /* a(4) */
    seq_cache[4] = 36; /* a(5) */
    seq_cache[5] = 76; /* a(6) */
    seq_size = 6;
}

seq_t a(idx_t n)
{
    /* n is 1‑based index; the function is valid for n >= 1 only. */
    if (n < 1) return 0;  /* undefined below offset, but return 0 */

    init_seq_cache();

    /* extend the cache up to n if necessary */
    if (n > seq_size) {
        ensure_seq_capacity(n);
        /* compute new terms using the recurrence */
        for (idx_t i = seq_size + 1; i <= n; ++i) {
            /* indices in seq_cache are 0‑based -> a(k) == seq_cache[k-1] */
            seq_t a1 = seq_cache[i-2];     /* a(i-1)  */
            seq_t a2 = seq_cache[i-3];     /* a(i-2)  */
            seq_t a3 = seq_cache[i-4];     /* a(i-3)  */
            seq_t a4 = seq_cache[i-5];     /* a(i-4)  */
            seq_t a5 = seq_cache[i-6];     /* a(i-5)  */
            seq_t a6 = seq_cache[i-7];     /* a(i-6)  */

            seq_cache[i-1] = a1 + a2 + (seq_t)3 * a3 + a4 - a5 - a6;
        }
        seq_size = n;
    }

    return seq_cache[n-1];
}
