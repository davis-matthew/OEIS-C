/*
 * A292479  p-INVERT of the positive squares, where p(S) = 1 - S^2.
 * https://oeis.org/A292479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1) - 14*a(n-2) + 22*a(n-3) - 14*a(n-4) + 6*a(n-5) - a(n-6),
   with a(0)=0, a(1)=1, a(2)=8, a(3)=35, a(4)=120, a(5)=392, a(6)=1336. */

#include <stdlib.h>

/* Cached sequence values.  The array is grown dynamically the first time
   an entry is requested.  Subsequent calls reuse the previously computed
   values. */
static seq_t *cache = NULL;          /* pointer to the cached terms */
static idx_t cached_len = 0;         /* number of terms currently stored */

/* Ensure that cache contains terms up to index n.  Uses the recurrence
   to compute any missing terms. */
static void ensure_cache(idx_t n)
{
    if (n < cached_len)            /* already computed */
        return;

    /* Grow the array to hold terms 0..n (new_len = n+1). */
    idx_t new_len = n + 1;
    /* realloc when cache is NULL uses malloc.  Static cast for C++ compliance. */
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache)
        abort();                   /* allocation failed */

    cache = new_cache;

    /* Initial values that do not depend on the recurrence. */
    static const seq_t init_vals[7] = {0, 1, 8, 35, 120, 392, 1336};

    /* Compute any new terms sequentially. */
    for (idx_t i = cached_len; i < new_len; ++i) {
        if (i < 7) {
            cache[i] = init_vals[i];
        } else {
            /* Terms needed for the recurrence. */
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t a4 = cache[i - 4];
            seq_t a5 = cache[i - 5];
            seq_t a6 = cache[i - 6];

            seq_t term1 = a1 * 6 + a3 * 22 + a5 * 6;   /* 6*a1 +22*a3 +6*a5 */
            seq_t term2 = a2 * 14 + a4 * 14 + a6;      /* 14*a2 +14*a4 +a6 */

            cache[i] = term1 - term2;                  /* a(i) */
        }
    }

    cached_len = new_len;
}

/* Public function returning the nth term of the sequence. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
