/*
 * A343894  Perimeters of integer-sided primitive triangles (a, b, c) where
 *          side a is the harmonic mean of the 2 other sides b and c, i.e.,
 *          2/a = 1/b + 1/c with b < a < c. The triples (a, b, c) are listed
 *          in increasing order of side a, and if sides a coincide, in
 *          increasing order of side b.
 * https://oeis.org/A343894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 47, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A343894: Perimeters of primitive integer‑sided triangles
 * (a, b, c) with a the harmonic mean of b and c,
 * 2/a = 1/b + 1/c, with b < a < c.
 * Triples are listed by increasing a and, for equal a, by increasing b.
 * The sequence is not monotonic; all terms are odd.
 *
 * This implementation enumerates all such triples in order and caches
 * the resulting perimeters.  Subsequent calls use the cache so the
 * function works in O(n) time for the n‑th term.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

/* --- Cache for perimeters --------------------------------------- */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* --- State for continuing enumeration --------------------------- */
static idx_t next_a = 1;         /* next value of a to be processed */

/* --- Push a new perimeter into the cache ----------------------- */
static void push_perimeter(seq_t perimeter)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* memory exhaustion */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = perimeter;
}

/* --- Return the n-th term ------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* sequence offset = 1 */

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        idx_t a_val = next_a;
        seq_t sa   = (seq_t)a_val;

        idx_t half = a_val / 2;
        for (idx_t b = half + 1; b < a_val; ++b) {
            seq_t sb = (seq_t)b;

            /* denominator d = 2b - a > 0 for chosen b */
            idx_t d_val = 2 * b - a_val;
            seq_t d = (seq_t)d_val;

            /* c = a*b / d */
            seq_t ab = sa * sb;
            if (ab % d != 0) continue;          /* c not integer */

            seq_t sc = ab / d;                   /* c */

            if (sc <= sa) continue;              /* requires a < c */
            if (sc <= sb) continue;              /* redundant but safe */
            seq_t diff = sc - sa;                /* c-a */
            if (diff >= sb) continue;            /* triangle inequality b + a > c */

            /* Primitive test */
            seq_t g = gcd(sa, sb);
            g = gcd(g, sc);
            if (g != (seq_t)1) continue;

            /* Push perimeter */
            push_perimeter(sa + sb + sc);
        }

        /* Move to next a after completing all b for current a */
        ++next_a;
    }

    return cache[n - 1];
}
