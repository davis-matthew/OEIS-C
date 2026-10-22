/*
 * A342387  Integers x such that there exists y, x>y>=1, satisfying
 *          (y+1)/(x+1) = (x||(y+1))/(y||(x+1)) where z||t is the
 *          concatenation of z and t.
 * https://oeis.org/A342387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A342387: integers x for which there exists y ≥ 1 such that
 *
 *     (y+1)/(x+1) = (x || (y+1)) / (y || (x+1))
 *
 * where z || t denotes decimal concatenation.  The equality is equivalent
 * to the Pell type equation
 *
 *          (2x+1)² – 10(2y+1)² = –9,
 *
 * whose solutions fall into three disjoint orbits generated from the
 * primitive solutions (9,3), (41,13) and (79,25).  For each orbit we
 * repeatedly multiply by the fundamental unit (19+6√10), test the
 * digit–length condition digit_len((u+1)/2) = digit_len((v+1)/2)+1, and
 * produce the corresponding x = ((u+1)/2) – 1.  The terms from all orbits
 * are merged in increasing order and cached so that arbitrary calls to
 * a(n) for n ≥ 1 are answered in O(log n) time. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_len.h"

typedef struct {
    seq_t u;          /* current u = 2x+1 of the Pell solution */
    seq_t v;          /* current v = 2y+1 of the Pell solution */
    seq_t next_x;     /* next term (x) to be returned from this orbit */
    bool ready;       /* true if next_x is ready */
} PellGen;

/* multiply (u + v√10) by the fundamental unit (19 + 6√10) */
static void pell_advance(PellGen *g)
{
    seq_t new_u = g->u * (seq_t)19 + g->v * (seq_t)60; /* 19*u + 60*v */
    seq_t new_v = g->u * (seq_t)6  + g->v * (seq_t)19; /* 6*u + 19*v */
    g->u = new_u;
    g->v = new_v;
}

/* ensure that next_x contains the next valid term from this orbit */
static void pell_ensure_next(PellGen *g)
{
    if (g->ready) return;

    while (true) {
        seq_t xu = (g->u + (seq_t)1) / (seq_t)2;  /* b = x+1 = (u+1)/2 */
        seq_t yu = (g->v + (seq_t)1) / (seq_t)2;  /* a = y+1 = (v+1)/2 */

        if (digit_len(xu) - digit_len(yu) == 1) {
            g->next_x = xu - (seq_t)1;   /* x = xu – 1 */
            g->ready   = true;
            pell_advance(g);             /* advance past the used solution */
            break;
        }
        pell_advance(g);
    }
}

/* cache for the sequence */
static seq_t  *cache   = NULL;
static size_t  cache_cap = 0;
static idx_t   cache_len = 0;

/* generators for the three primitive orbits */
static PellGen gens[3];
static bool     initialized = false;

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety check – not part of the sequence */

    if (!initialized) {
        /* initialise generators with the three primitive solutions */
        gens[0].u = (seq_t)9;   gens[0].v = (seq_t)3;   gens[0].ready = false;
        gens[1].u = (seq_t)41;  gens[1].v = (seq_t)13;  gens[1].ready = false;
        gens[2].u = (seq_t)79;  gens[2].v = (seq_t)25;  gens[2].ready = false;
        initialized = true;
    }

    /* extend the cache until it contains at least n terms */
    while (cache_len < n) {
        /* ensure each generator has its next term ready */
        pell_ensure_next(&gens[0]);
        pell_ensure_next(&gens[1]);
        pell_ensure_next(&gens[2]);

        /* find the smallest next term among the generators */
        int min_i = 0;
        if (gens[1].next_x < gens[min_i].next_x) min_i = 1;
        if (gens[2].next_x < gens[min_i].next_x) min_i = 2;

        /* append the smallest term to the cache */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) abort();  /* out of memory */
            cache = new_cache;
            cache_cap = new_cap;
        }
        cache[cache_len++] = gens[min_i].next_x;

        /* invalidate the generator from which the term was taken */
        gens[min_i].ready = false;
    }

    return cache[n-1];
}
