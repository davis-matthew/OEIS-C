/*
 * A302682  Number of 4Xn 0..1 arrays with every element equal to 0, 1, 4 or 6
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) +5*a(n-3) +5*a(n-4) * -3*a(n-5) -3*a(n-6)
 *            +2*a(n-7) for n>11 * Initial values (n = 1..11): * a(1)=8,
 *            a(2)=6, a(3)=7, a(4)=20, a(5)=30, * a(6)=76, a(7)=217, a(8)=509,
 *            a(9)=1189, a(10)=3034, a(11)=7569
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 50, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t init_vals[11] = {
    (seq_t)8,  (seq_t)6,  (seq_t)7,  (seq_t)20, (seq_t)30,
    (seq_t)76, (seq_t)217,(seq_t)509,(seq_t)1189,(seq_t)3034,(seq_t)7569
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;     /* capacity in seq_t elements */
static size_t cache_len = 0;     /* number of valid entries stored */

static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    if (cache == NULL) {
        cache = (seq_t*)calloc(new_cap, sizeof(seq_t));
    } else {
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    }
    if (cache == NULL) {
        /* allocation failed – abort */
        exit(1);
    }
    /* initialize newly allocated slots to zero (already zeroed with calloc or not needed) */
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for the sequence, but safe to return 0 */
    if (n <= 11) return init_vals[n-1];

    /* ensure cache holds at least 11 values */
    if (cache_len == 0) {
        ensure_capacity(16);       /* start with a small capacity */
        for (size_t i = 0; i < 11; ++i) cache[i] = init_vals[i];
        cache_len = 11;
    }

    /* extend cache as needed */
    if (cache_len < (size_t)n) {
        size_t target = (size_t)n;
        if (target > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < target) new_cap *= 2;
            ensure_capacity(new_cap);
        }
        for (size_t i = cache_len; i < target; ++i) {
            /* i is zero‑based index, so current n = i+1 */
            seq_t val = cache[i-1]            /* a(i)   = a(i-1)   */
                       + cache[i-2]          /*          + a(i-2)   */
                       + 5 * cache[i-3]      /*          + 5*a(i-3) */
                       + 5 * cache[i-4];     /*          + 5*a(i-4) */
            /* subtract terms */
            val = val - 3 * cache[i-5];      /*            -3*a(i-5) */
            val = val - 3 * cache[i-6];      /*            -3*a(i-6) */
            val = val + 2 * cache[i-7];      /*            +2*a(i-7) */
            cache[i] = val;
        }
        cache_len = target;
    }

    return cache[n-1];
}
