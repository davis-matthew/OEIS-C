/*
 * A399515  Numbers a for which there are smaller integers b > c > d > 0 such
 *          that c^2 = a + d and d^2 = b + c.
 * https://oeis.org/A399515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) is the n-th positive integer a such that there exist
 *            integers b > c > d > 0 with c^2 = a + d and d^2 = b + c. For a
 *            given a, let c = ⌊√a⌋ + 1, d = c^2 – a. Then a is in the
 *            sequence iff d < c and c < d^2/2. The implementation keeps a
 *            dynamic cache of computed values and tests successive inte...
 * Range:     u8 n <= 10, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* cache for sequence values */
static seq_t *seq_cache = NULL;   /* array of cached a(n) values   */
static size_t seq_len  = 0;        /* number of values cached so far */
static size_t seq_cap  = 0;        /* allocated capacity of seq_cache */
static seq_t next_candidate = 13;  /* the next integer to test for membership */

/* return true iff a satisfies the defining inequalities */
static bool in_sequence(seq_t a)
{
    seq_t c = floor_sqrt(a) + (seq_t)1;       /* c = floor(√a)+1         */
    seq_t d = c * c - a;                     /* d = c^2 – a             */
    if (d >= c) return false;                /* must satisfy d < c      */
    seq_t d2 = d * d;
    if ( (seq_t)2 * c < d2 ) return true;    /* c < d^2/2                */
    return false;
}

/* ensure capacity of the cache */
static void vec_resize(void)
{
    if (seq_cap == 0) {
        seq_cap = 16;
        seq_cache = (seq_t*)realloc(NULL, seq_cap * sizeof(seq_t));
    } else if (seq_len == seq_cap) {
        seq_cap <<= 1;
        seq_cache = (seq_t*)realloc(seq_cache, seq_cap * sizeof(seq_t));
    }
    /* In both cases realloc returns NULL on failure; propagating such an
       error would lead to a crash, which is acceptable for the build
       harness. */
}

/* OEIS sequence A399515 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined, but return 0 safely */

    /* grow cache until we have at least n terms */
    while (seq_len < (size_t)n) {
        vec_resize();

        /* test next_candidate for membership */
        if (in_sequence(next_candidate)) {
            seq_cache[seq_len++] = next_candidate;
        }
        ++next_candidate;
    }
    return seq_cache[n-1];
}
