/*
 * A355489  Numbers k such that A000120(k) = A007814(k) + 2.
 * https://oeis.org/A355489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that A000120(k) = A007814(k) + 2
   (popcount(k) = v2(k) + 2).  a(n) returns the n‑th such integer,
   a(1) = 3, a(2) = 5, … as listed in the OEIS entry. */

#include <stdlib.h>

static seq_t *cache = NULL;     /* store found terms */
static size_t cache_cap = 0;    /* current allocation size */
static size_t cache_len = 0;    /* number of elements stored */
static seq_t next_candidate = 1;/* next integer to test */

/* Count trailing zeros of a seq_t value */
static unsigned trailing_zeros(seq_t x)
{
    if (x == 0) return 0;
    unsigned tz = 0;
    while ((x & 1u) == 0) {      /* use uint literal for consistency */
        tz++;
        x >>= 1;
    }
    return tz;
}

/* Count ones in the binary expansion of a seq_t value */
static unsigned popcount(seq_t x)
{
    unsigned pc = 0;
    while (x) {
        pc++;
        x &= (x - 1);            /* clear lowest set bit */
    }
    return pc;
}

/* Ensure the cache has at least new_cap slots */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    size_t cap = cache_cap ? cache_cap * 2 : 16;
    if (cap < new_cap) cap = new_cap;
    seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!tmp) exit(1);           /* allocation failure – abort */
    cache = tmp;
    cache_cap = cap;
}

seq_t a(idx_t n)
{
    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        /* find next integer satisfying popcount(k) = v2(k)+2 */
        while (1) {
            unsigned tz = trailing_zeros(next_candidate);
            unsigned pc = popcount(next_candidate);
            if (pc == tz + 2) {
                /* store the term */
                grow_cache(cache_len + 1);
                cache[cache_len++] = next_candidate;
                next_candidate++;   /* start searching from the next integer */
                break;
            }
            next_candidate++;
        }
    }
    return cache[n - 1];   /* a(1) is cached[0] */
}
