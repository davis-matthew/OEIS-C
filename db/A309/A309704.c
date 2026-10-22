/*
 * A309704  a(1) = 3, a(2) = 4, a(3) = 5, a(4) = 4, a(5) = 5; a(6) = 6; a(n) =
 *          a(n-a(n-1)) + a(n-a(n-4)) for n > 6.
 * https://oeis.org/A309704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, fini
 * Formula:   a(n) = a(n-a(n-1)) + a(n-a(n-4)) for n > 6 with initial values:
 *            a(1)=3, a(2)=4, a(3)=5, a(4)=4, a(5)=5, a(6)=6
 * Range:     u8 n <= 494, u16 n <= 9391, u32 n <= 9391, u64 n <= 9391,
 *            u128 n <= 9391, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* memoised values, 0‑based: cache[i] = a(i+1) */
static size_t cap   = 0;         /* current capacity of cache          */
static idx_t  computed = 0;      /* largest n for which a(n) is stored */

/* enlarge cache to hold at least n elements (n is 1‑based index) */
static void ensure_capacity(idx_t n)
{
    if (cap >= n) return;
    size_t new_cap = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();  /* allocation failed */
    cache = tmp;
    cap   = new_cap;
}

/* OEIS sequence A309704 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n = 0, safety */

    /* initialise first 6 terms only once */
    if (!cache) {
        ensure_capacity(6);
        cache[0] = (seq_t)3;    /* a(1) */
        cache[1] = (seq_t)4;    /* a(2) */
        cache[2] = (seq_t)5;    /* a(3) */
        cache[3] = (seq_t)4;    /* a(4) */
        cache[4] = (seq_t)5;    /* a(5) */
        cache[5] = (seq_t)6;    /* a(6) */
        computed = 6;
    }

    if (n <= computed) return cache[n-1];

    /* grow storage if needed */
    ensure_capacity(n);

    /* compute missing values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* a(i-1) and a(i-4) are already known because i-1 >= computed+1 - 1 = computed
           and i-4 >= 2 for i >= 7. */
        seq_t a_im1     = cache[i-2];          /* a(i-1)  */
        seq_t a_im4     = cache[i-5];          /* a(i-4)  */
        idx_t idx1      = i - (idx_t)a_im1;    /* i - a(i-1)  */
        idx_t idx2      = i - (idx_t)a_im4;    /* i - a(i-4)  */
        cache[i-1] = cache[(size_t)idx1-1] + cache[(size_t)idx2-1];
    }

    computed = n;
    return cache[n-1];
}
