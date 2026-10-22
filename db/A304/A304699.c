/*
 * A304699  Number of nX3 0..1 arrays with every element unequal to 1, 2, 4, 5
 *          or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 52, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A304699
   Number of n×3 0..1 arrays with every element unequal to
   1, 2, 4, 5 or 6 king‑move adjacent elements, with upper left
   element zero.

   Empirical recurrence for n>20:
     a(n) = a(n-1) + 2*a(n-2) + 10*a(n-3) - a(n-4) -20*a(n-5) -48*a(n-6)
            -31*a(n-7) +56*a(n-8) +107*a(n-9) +109*a(n-10)
            -30*a(n-11) -92*a(n-12) -70*a(n-13) -9*a(n-14)
            -35*a(n-15) -32*a(n-16) -4*a(n-17)
   Initial values: a(1)=1, a(2)=11, a(3)=8, a(4)=21, a(5)=46, a(6)=103,
                   a(7)=201, a(8)=512, a(9)=1123, a(10)=2536,
                   a(11)=6010, a(12)=14033, a(13)=32343, a(14)=76620,
                   a(15)=180379, a(16)=423657, a(17)=1000938,
                   a(18)=2367196, a(19)=5593222, a(20)=13232605
*/
#include <stdlib.h>

static const seq_t init[20] = {
    1, 11, 8, 21, 46, 103, 201, 512, 1123, 2536,
    6010, 14033, 32343, 76620, 180379, 423657,
    1000938, 2367196, 5593222, 13232605
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Grow the cache up to at least n terms */
static void ensure_up_to(idx_t n)
{
    /* nothing to do if we already have enough terms */
    if (n <= cache_len) return;

    /* grow memory if needed */
    if (n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 20) {
            /* initial values */
            cache[i - 1] = init[i - 1];
        } else {
            /* recurrence for n > 20 */
            size_t k = (size_t)(i - 1);          /* zero‑based index (a(1) is cache[0]) */
            seq_t val = cache[k - 1];            /* a(n-1)  */
            val += 2 * cache[k - 2];             /* 2*a(n-2)*/
            val += 10 * cache[k - 3];            /* 10*a(n-3)*/
            val -= cache[k - 4];                 /* -a(n-4) */
            val -= 20 * cache[k - 5];            /* -20*a(n-5) */
            val -= 48 * cache[k - 6];            /* -48*a(n-6) */
            val -= 31 * cache[k - 7];            /* -31*a(n-7) */
            val += 56 * cache[k - 8];            /* +56*a(n-8) */
            val += 107 * cache[k - 9];           /* +107*a(n-9) */
            val += 109 * cache[k - 10];          /* +109*a(n-10) */
            val -= 30 * cache[k - 11];           /* -30*a(n-11) */
            val -= 92 * cache[k - 12];           /* -92*a(n-12) */
            val -= 70 * cache[k - 13];           /* -70*a(n-13) */
            val -= 9 * cache[k - 14];            /* -9*a(n-14) */
            val -= 35 * cache[k - 15];           /* -35*a(n-15) */
            val -= 32 * cache[k - 16];           /* -32*a(n-16) */
            val -= 4 * cache[k - 17];            /* -4*a(n-17) */
            cache[k] = val;
        }
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* below offset — arbitrary */
    ensure_up_to(n);
    return cache[n - 1];
}
