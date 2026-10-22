/*
 * A392742  A 4-automatic binary sequence with irreducible nested recurrence
 *          (see Comments).
 * https://oeis.org/A392742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=0, a(1)=1, a(4n)=a(n), a(4n+1)=1-a(n),
   a(4n+2)=a(2n+a(n)), a(4n+3)=1-a(2n+a(n)) */
#include <stdlib.h>
#include <stdbool.h>

/* optional: if seq_alloc or seq_realloc were provided by the utils,
   the following code would use them, but we use standard malloc/realloc */

static seq_t *cache   = NULL;          /* cache of a(n) values       */
static unsigned char *known = NULL;    /* known[n] = 1 iff cache[n] valid */
static size_t cache_cap = 0;           /* current capacity of the arrays */

/* Ensure that cache and known are large enough for index n */
static void ensure_cache(idx_t n)
{
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        /* enlarge cache array */
        if (cache == NULL) {
            cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        } else {
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        }
        /* enlarge known array */
        if (known == NULL) {
            known = (unsigned char *)malloc(sizeof(unsigned char) * new_cap);
        } else {
            known = (unsigned char *)realloc(known, sizeof(unsigned char) * new_cap);
        }
        /* initialise newly added entries as unknown */
        for (size_t i = cache_cap; i < new_cap; ++i) {
            cache[i] = (seq_t)0;   /* value is irrelevant until computed */
            known[i] = 0;
        }
        cache_cap = new_cap;
    }
}

/* Recursive computation of a(n) with memoisation */
seq_t a(idx_t n)
{
    ensure_cache(n);
    if (known[n]) return cache[n];

    if (n == 0) {
        cache[n] = (seq_t)0;
        known[n] = 1;
        return cache[n];
    }
    if (n == 1) {
        cache[n] = (seq_t)1;
        known[n] = 1;
        return cache[n];
    }

    idx_t k = n >> 2;            /* n / 4   */
    idx_t r = n & 3;             /* n % 4   */
    seq_t val;

    if (r == 0) {
        val = a(k);                 /* a(4k)   = a(k) */
    } else if (r == 1) {
        val = a(k) ^ (seq_t)1;      /* a(4k+1) = 1 - a(k) */
    } else {
        seq_t ak = a(k);                   /* a(k)    */
        idx_t idx2 = 2*k + (idx_t)ak;      /* 2k + a(k) */
        if (r == 2) {
            val = a(idx2);                 /* a(4k+2) = a(2k+a(k)) */
        } else { /* r == 3 */
            val = a(idx2) ^ (seq_t)1;      /* a(4k+3) = 1 - a(2k+a(k)) */
        }
    }

    cache[n] = val;
    known[n] = 1;
    return val;
}
