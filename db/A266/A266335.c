/*
 * A266335  G.f. = b(2)^2*b(6)/(x^7+x^6-x^5-x^2-x+1), where b(k) =
 *          (1-x^k)/(1-x).
 * https://oeis.org/A266335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-5)-a(n-6)-a(n-7) for n ≥ 8 with a(0)=1,
 *            a(1)=4, a(2)=9, a(3)=17, a(4)=30, a(5)=52, a(6)=88, a(7)=145.
 *            (Derived from the generating function G.f. =
 *            b(2)^2*b(6)/(x⁷+x⁶‑x⁵‑x²‑x+1).)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  Cached sequence storage                                          */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;   /* array storing computed a(n) values   */
static size_t cap   = 0;      /* allocated capacity (in terms)        */
static size_t len   = 0;      /* number of valid terms already stored */

/* Ensure that at least 'needed' slots are allocated.                */
static void ensure_capacity(size_t needed)
{
    if (cap < needed) {
        size_t new_cap = cap ? cap * 2 : 16;
        if (new_cap < needed)
            new_cap = needed;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Convert index to a size_t usable for array access             */
    size_t idx = (size_t)n;

    /* Allocate storage for the requested term                       */
    ensure_capacity(idx + 1);

    /* Initialize the first eight terms only once                     */
    if (len == 0) {
        static const seq_t init[8] = {
            (seq_t)1,   /* a(0) */
            (seq_t)4,   /* a(1) */
            (seq_t)9,   /* a(2) */
            (seq_t)17,  /* a(3) */
            (seq_t)30,  /* a(4) */
            (seq_t)52,  /* a(5) */
            (seq_t)88,  /* a(6) */
            (seq_t)145  /* a(7) */
        };
        ensure_capacity(8);
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init[i];
        len = 8;
    }

    /* Generate missing terms up to 'idx' using the recurrence        */
    if (len <= idx) {
        for (size_t i = len; i <= idx; ++i) {
            /* a(i) = a(i-1)+a(i-2)+a(i-5)-a(i-6)-a(i-7)              */
            seq_t val = cache[i-1] + cache[i-2]
                      + cache[i-5] - cache[i-6] - cache[i-7];
            cache[i] = val;
        }
        len = idx + 1;
    }

    return cache[idx];
}
