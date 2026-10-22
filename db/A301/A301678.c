/*
 * A301678  Coordination sequence for node of type V1 in "krn" 2-D tiling (or
 *          net).
 * https://oeis.org/A301678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: 
 * a(0)=1,a(1)=5,a(2)=10,a(3)=14,a(4)=20,a(5)=26,a(6)=31,a(7)=36,a(8)=41,a(9)=46;
 * for n>=10: 
 *   a(n)=a(n-1)-a(n-2)+a(n-3)+a(n-7)-a(n-8)+a(n-9)-a(n-10)+N_n,
 * where N_n = 1,4,6,8,11,10,11,10,6,4,3,0,-2 for n=0..12, and N_n=0 for n>12.
 * This recurrence follows from the generating function:
 * G(x) = -(2*x^12-3*x^10-4*x^9-6*x^8-10*x^7-11*x^6-10*x^5-11*x^4-8*x^3-6*x^2-4*x-1) /
 *        (x^10-x^9+x^8-x^7-x^3+x^2-x+1).
 */

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_sz = 0;
static size_t cache_cap = 0;

/* Initial 10 terms of the sequence */
static const seq_t seed[10] = {1, 5, 10, 14, 20, 26, 31, 36, 41, 46};

/* Values of N_n for n = 0 .. 12 */
static const int N_int[13] = {1, 4, 6, 8, 11, 10, 11, 10, 6, 4, 3, 0, -2};

static void init_cache(void) {
    if (cache != NULL) return;
    cache_cap = 16;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) return;      /* out of memory – not expected for test data */
    for (size_t i = 0; i < 10; ++i) cache[i] = seed[i];
    cache_sz = 10;
}

static void ensure_capacity(size_t needed) {
    if (needed < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= needed) new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) return;    /* allocation failed – not expected */
    cache = new_ptr;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    init_cache();
    size_t idx = (size_t)n;

    if (idx < cache_sz) return cache[idx];

    ensure_capacity(idx + 1);
    for (size_t k = cache_sz; k <= idx; ++k) {
        seq_t val;
        if (k < 10) {
            val = seed[k];
        } else {
            /* a(k) = a(k-1) - a(k-2) + a(k-3) + a(k-7) - a(k-8) + a(k-9) - a(k-10) + N_k */
            val =  cache[k-1] + cache[k-3] + cache[k-7] + cache[k-9];
            val -= cache[k-2];
            val -= cache[k-8];
            val -= cache[k-10];
            int Nk = (k <= 12) ? N_int[k] : 0;
            if (Nk > 0) val += (seq_t)Nk;
            else if (Nk < 0) val -= (seq_t)(-Nk);
        }
        cache[k] = val;
    }
    cache_sz = idx + 1;
    return cache[idx];
}
