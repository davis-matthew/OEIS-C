/*
 * A374654  a(n) = Product_{k=0..n} (L(k)+1), where L=A000032 (Lucas numbers).
 * https://oeis.org/A374654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (L(k)+1), where L(k) are Lucas numbers
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute Lucas numbers and this sequence with memoisation.
   The arrays are grown as needed and each entry is computed only once,
   so out-of-order calls are fine. */

static seq_t *lucas_cache = NULL;   /* L(k) */
static size_t lucas_cap = 0;        /* capacity of lucas_cache */
static idx_t  lucas_last = 0;       /* last computed index in lucas_cache */

static seq_t *seq_cache  = NULL;   /* a(n) values */
static size_t seq_cap  = 0;        /* capacity of seq_cache */
static idx_t  seq_last = 0;        /* last computed index in seq_cache */

/* Ensure lucas_cache has room for indices up to m (inclusive). */
static void ensure_lucas_cap(idx_t m)
{
    if (lucas_cap > 0 && lucas_cap > (size_t)m+1) return;
    size_t new_cap = lucas_cap ? lucas_cap : 2;
    while (new_cap < (size_t)m+1) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(lucas_cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();          /* memory allocation failed */
    lucas_cache = new_arr;
    lucas_cap = new_cap;
}

/* Ensure seq_cache has room for indices up to m (inclusive). */
static void ensure_seq_cap(idx_t m)
{
    if (seq_cap > 0 && seq_cap > (size_t)m+1) return;
    size_t new_cap = seq_cap ? seq_cap : 2;
    while (new_cap < (size_t)m+1) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    seq_cache = new_arr;
    seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialise in the very first call. */
    if (lucas_cap == 0) {
        ensure_lucas_cap(1);
        lucas_cache[0] = (seq_t)2;                    /* L(0) */
        lucas_cache[1] = (seq_t)1;                    /* L(1) */
        lucas_last = 1;
    }
    if (seq_cap == 0) {
        ensure_seq_cap(0);
        seq_cache[0] = lucas_cache[0] + (seq_t)1;     /* a(0) = L(0)+1 */
        seq_last = 0;
    }

    /* Extend Lucas cache up to n. */
    if ((size_t)n > lucas_last) {
        ensure_lucas_cap(n);
        for (idx_t i = lucas_last + 1; i <= n; ++i) {
            lucas_cache[i] = lucas_cache[i-1] + lucas_cache[i-2];
        }
        lucas_last = n;
    }

    /* Extend sequence cache up to n. */
    if ((size_t)n > seq_last) {
        ensure_seq_cap(n);
        for (idx_t i = seq_last + 1; i <= n; ++i) {
            seq_cache[i] = seq_cache[i-1] * (lucas_cache[i] + (seq_t)1);
        }
        seq_last = n;
    }

    return seq_cache[n];
}
