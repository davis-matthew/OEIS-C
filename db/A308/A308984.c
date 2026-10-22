/*
 * A308984  If a(n) is not a term of a(0)..a(n-1), then a(n+1) = |a(n) -
 *          a(n-1)|, otherwise a(n+1) = a(n) + n - m, where a(m) = a(n), m <
 *          n, and m is maximal. a(0)=0, a(1)=1.
 * https://oeis.org/A308984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A308984:
   a(0) = 0, a(1) = 1.
   For n ≥ 2:
      Let p  = a(n-1)   (previous term),
          pp = a(n-2)   (term before that).
      Let m be the largest index < n-1 with a(m) = p
      If no such m exists: a(n) = |p – pp|
      Else:               a(n) = p + (n-1) – m
   (indices are 0‑based). */

#include <stdint.h>
#include <stdlib.h>

/* maintain a dynamic cache of computed terms */
static seq_t   *cache      = NULL;    /* array of terms */
static idx_t   cache_size  = 0;       /* number of terms currently stored */
static idx_t   cache_cap   = 0;       /* allocated capacity (elements) */

static void ensure_capacity(idx_t idx) {
    if (cache_cap > idx) return;

    idx_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= idx) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* in C, realloc(void*) can be assigned directly; in C++ a cast is required
       but this cast works in both C and C++. */
    cache = tmp;
    cache_cap = new_cap;
}

static void compute_to(idx_t idx) {
    if (idx < cache_size) return;            /* already computed */

    ensure_capacity(idx);
    /* initialize base cases if not yet done */
    if (cache_size == 0) {
        cache[0] = (seq_t)0;
        cache_size = 1;
    }
    if (cache_size == 1 && idx >= 1) {
        cache[1] = (seq_t)1;
        cache_size = 2;
    }

    /* extend cache to required index */
    for (idx_t i = cache_size; i <= idx; ++i) {
        if (i < 2) continue;           /* base terms already set */

        seq_t p  = cache[i-1];
        seq_t pp = cache[i-2];

        /* find largest j < i-1 with cache[j] == p */
        idx_t m = (idx_t)-1;            /* -1 means “not found” */
        for (idx_t j = i-2; ; --j) {
            if (cache[j] == p) { m = j; break; }
            if (j == 0) break;
        }

        seq_t next;
        if (m == (idx_t)-1) {                     /* no previous occurrence */
            /* absolute difference */
            next = (p > pp) ? p - pp : pp - p;
        } else {                                  /* repeated value */
            next = p + (seq_t)(i-1) - (seq_t)m;
        }
        cache[i] = next;
    }
    cache_size = idx + 1;
}

/* public interface */
seq_t a(idx_t n) {
    compute_to(n);
    return cache[n];
}
