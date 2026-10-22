/*
 * A372780  Numbers m such that v^n - u^m >= u^(m+1) - v^n, where u=2, v=3,
 *          and u^m < v^n < u^(m+1).
 * https://oeis.org/A372780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer k such that 2*3^k >= 3*2^floor(k·log₂3). This
 *            is equivalent to requiring that the fractional part of k·log₂3
 *            is at least log₂(3/2)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

static seq_t *cache   = NULL;   /* cached values of a(n) */
static size_t cached  = 0;      /* number of cached terms */
static size_t cap     = 0;      /* capacity of cache */
static idx_t next_k   = 1;      /* next integer k to test */
static bool init_done = false;  /* whether initialisation has been performed */

static void init_once(void)
{
    if (init_done) return;
    init_done = true;

    cap   = 64;
    cache = (seq_t *)malloc(cap * sizeof(seq_t));
    /* cache[0] will be filled on the first call to ensure() */
    cached = 0;
    next_k = 1;
}

/* Ensure that cache contains at least n terms. */
static void ensure(idx_t n)
{
    init_once();

    /* constants used in the fractional‑part test */
    static const long double alpha = log2l(3.0L);          /* log₂3         */
    static const long double beta  = log2l(1.5L);          /* log₂(3/2)     */
    static const long double eps   = 1e-18L;               /* safety margin */

    while (cached < n) {
        /* Compute fractional part of k·alpha */
        long double val = (long double)next_k * alpha;
        idx_t floor_val = (idx_t)val;                     /* floor of val  */
        long double frac   = val - (long double)floor_val;

        if (frac >= beta - eps) {                         /* inequality holds */
            if (cached == cap) {                          /* grow cache if needed */
                cap <<= 1;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[cached++] = (seq_t)next_k;
        }
        ++next_k;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for n = 0, but safe guard */
    ensure(n);
    return cache[n - 1];
}
