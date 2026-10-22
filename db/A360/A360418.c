/*
 * A360418  Numbers k such that, in a listing of all congruence classes of
 *          positive integers, the k-th congruence class contains k. Here the
 *          class r mod m (with r in {1,...,m}) precedes the class a' mod b'
 *          (with r' in {1,...,m'}) iff m < m' or r > r'.
 * https://oeis.org/A360418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 362, u32 n <= 9872, u64 n <= 9872,
 *            u128 n <= 9872, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A360418
 *
 *  Formula (recurrence):  a(n) = a(n-1) + 2*a(n-4) - 2*a(n-5) - a(n-8) + a(n-9)
 *  for n > 8, with initial values
 *      a(1)=1, a(2)=2, a(3)=3, a(4)=5, a(5)=13,
 *      a(6)=17, a(7)=20, a(8)=25, a(9)=41.
 *
 *  This implementation memoises the sequence in a static array and
 *  grows it as needed.  It works for any seq_t type (8‑, 16‑, 32‑, 64‑,
 *  128‑bit unsigned or arbitrary‑precision) and any idx_t type (here
 *  uint64_t).  Non‑existent indices (n==0) return 0.
 *
 *  The code is valid both as C11 and as C++17.
 */

#include <stdlib.h>  /* malloc, realloc, abort */
#include <stddef.h>  /* size_t */

/* Cached values for the sequence, 0‑based (cache[0] = a(1)). */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* The first nine terms of the sequence. */
static const seq_t base[9] = {
    (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)5, (seq_t)13,
    (seq_t)17, (seq_t)20, (seq_t)25, (seq_t)41
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* outside the sequence */

    size_t idx = (size_t)n;                /* number of terms needed */

    /* Grow the cache if required. */
    if (idx > cache_len) {
        /* Resize the buffer. */
        seq_t *tmp = (seq_t *)realloc(cache, idx * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failure */
        cache = tmp;

        /* Initialise new entries that belong to the base block. */
        size_t i;
        if (cache_len < 9)                 /* only need to set missing ones */
            for (i = cache_len; i < idx && i < 9; ++i)
                cache[i] = base[i];

        /* Compute all remaining terms using the recurrence. */
        for (i = cache_len; i < idx; ++i) {
            if (i < 9) continue;          /* already initialised */
            cache[i] =
                cache[i-1]                    /* a(n-1)   */
              + (seq_t)2 * cache[i-4]          /* +2*a(n-4) */
              - (seq_t)2 * cache[i-5]          /* -2*a(n-5) */
              -               cache[i-8]       /* -a(n-8)   */
              +               cache[i-9];      /* +a(n-9)   */
        }
        cache_len = idx;
    }

    return cache[n-1];                     /* a(1) is at cache[0] */
}
