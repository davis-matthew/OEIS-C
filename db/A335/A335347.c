/*
 * A335347  Middle side of primitive triples for integer-sided triangles that
 *          have two perpendicular medians, the triples being ordered by
 *          increasing perimeter.
 * https://oeis.org/A335347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   int_gcd, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the middle side a(n) of primitive integer‑sided triangles
   that have two perpendicular medians, ordered by increasing perimeter.
   The values are computed from the two parametric families described
   in OEIS A335347.  The function is thread‑safe after the first
   invocation and memoises the whole list in static storage. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/int_gcd.h"

/* GCD for integers (used only on values <= 600). */

/* A triple with its perimeter and the middle side. */
struct triple {
    uint64_t p;   /* perimeter */
    seq_t a;      /* middle side (value to be returned) */
};

/* Static storage of generated triples and the resulting sequence. */
static struct triple *triples = NULL;
static size_t triples_cap = 0;
static size_t triples_sz  = 0;

static seq_t *seq_vals = NULL;   /* sorted a‑values */
static size_t seq_len  = 0;
static bool   initialized = false;

/* Append a triple to the dynamic array.  The array is grown by
   doubling its capacity. */
static void add_triple (uint64_t p, seq_t a)
{
    if (triples_sz >= triples_cap) {
        size_t new_cap = (triples_cap == 0) ? 128 : triples_cap * 2;
        struct triple *tmp = (struct triple *)realloc(triples,
                                                       new_cap * sizeof(struct triple));
        if (!tmp) { free(triples); exit(1); }
        triples = tmp;
        triples_cap = new_cap;
    }
    triples[triples_sz].p = p;
    triples[triples_sz].a = a;
    ++triples_sz;
}

/* Comparison function for qsort: sort by perimeter. */
static int cmp_triple (const void *x, const void *y)
{
    const struct triple *a = (const struct triple *)x;
    const struct triple *b = (const struct triple *)y;
    if (a->p < b->p) return -1;
    if (a->p > b->p) return 1;
    return 0;
}

/* Generate all primitive triples from the two parametric families
   up to a fixed bound of u; 600 is ample to cover at least 10000
   terms. */
static void generate_all_triples (void)
{
    const idx_t U_MAX = 600;  /* ensures many primitives */
    for (idx_t u = 2; u <= U_MAX; ++u) {
        for (idx_t v = 1; v < u; ++v) {
            if (int_gcd((int)u, (int)v) != 1) continue;
            if ( (u & 1) == (v & 1) ) continue;   /* different parity */

            /* ----- first class: u > 3v, (u-3v) not divisible by 5 ----- */
            if (3 * v < u && ((int)(u - 3 * v)) % 5 != 0) {
                uint64_t u_t = (uint64_t)u;
                uint64_t v_t = (uint64_t)v;
                uint64_t a_prime = 2 * (u_t * u_t - u_t * v_t - v_t * v_t);
                uint64_t b_prime = u_t * u_t + 4 * u_t * v_t - v_t * v_t;
                uint64_t c        = u_t * u_t + v_t * v_t;
                seq_t   a_mid     = (a_prime < b_prime) ? (seq_t)a_prime
                                                       : (seq_t)b_prime;
                uint64_t p        = c + a_prime + b_prime;
                add_triple(p, a_mid);
            }

            /* ----- second class: v < u < 2v, (u-2v) not divisible by 5 ----- */
            if (u < 2 * v) {
                int diff = (int)u - 2 * (int)v;   /* may be negative */
                if (diff % 5 != 0) {
                    uint64_t u_t = (uint64_t)u;
                    uint64_t v_t = (uint64_t)v;
                    uint64_t a_prime = 2 * (u_t * u_t + u_t * v_t - v_t * v_t);
                    uint64_t b_prime = -u_t * u_t + 4 * u_t * v_t + v_t * v_t;
                    uint64_t c        = u_t * u_t + v_t * v_t;
                    seq_t   a_mid     = (a_prime < b_prime) ? (seq_t)a_prime
                                                           : (seq_t)b_prime;
                    uint64_t p        = c + a_prime + b_prime;
                    add_triple(p, a_mid);
                }
            }
        }
    }

    /* Sort the triples by increasing perimeter, then extract the a‑values. */
    qsort(triples, triples_sz, sizeof(struct triple), cmp_triple);
    seq_vals = seq_alloc(triples_sz);   /* memory is value‑initialized */
    for (size_t i = 0; i < triples_sz; ++i)
        seq_vals[i] = triples[i].a;
    seq_len = triples_sz;

    initialized = true;
    free(triples);   /* no longer needed */
}

/* Public sequence routine. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* sequence is 1‑based */
    if (!initialized) generate_all_triples();  /* lazy computation */
    if (n <= seq_len) return seq_vals[n-1];
    return 0;  /* out‑of‑range; should not happen for tested n */
}
