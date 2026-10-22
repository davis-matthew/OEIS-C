/*
 * A316690  Number of nX5 0..1 arrays with every element unequal to 0, 1, 2,
 *          6, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A316690: Number of n×5 0..1 arrays with upper left element zero and no element having the number of king–move adjacent 1's equal to 0,1,2,6,7 or 8.
   Recurrence (n > 11):
     a(n) = a(n-1) + 3*a(n-2) + 12*a(n-3) + 2*a(n-4)
            - 9*a(n-5) - 27*a(n-6) - 6*a(n-7)
   Initial values:
     a(1)=16, a(2)=8, a(3)=21, a(4)=44, a(5)=88,
     a(6)=359, a(7)=1125, a(8)=3117, a(9)=10022,
     a(10)=31545, a(11)=95424. */
#include <stdlib.h>

/* static cache for computed terms a(1)..a(cache_n) */
static seq_t *cache = NULL;
static idx_t  cache_n = 0;

/* hard‑coded initial values 1..11 (zero‑based indices 0..10) */
static const seq_t init_vals[11] = {
    (seq_t)16,  (seq_t)8,   (seq_t)21,  (seq_t)44,  (seq_t)88,
    (seq_t)359, (seq_t)1125,(seq_t)3117,(seq_t)10022,(seq_t)31545,(seq_t)95424
};

/* Ensure cache has space for terms up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache_n >= n) return;                 /* already cover needed range */

    idx_t new_n = n;
    seq_t *tmp = (seq_t *)realloc(cache, new_n * sizeof(seq_t));
    if (!tmp) exit(1);                        /* out‑of‑memory */
    cache = tmp;

    /* Fill in the first 11 terms if they are not yet available */
    if (cache_n < 11) {
        for (idx_t i = cache_n; i < 11 && i < new_n; ++i)
            cache[i] = init_vals[i];
    }

    /* Compute terms beyond the initial segment */
    idx_t start = (cache_n > 11) ? cache_n : 11;
    for (idx_t i = start; i < new_n; ++i) {
        seq_t val  = cache[i-1];
        val += 3 * cache[i-2];
        val += 12 * cache[i-3];
        val += 2 * cache[i-4];
        val -= 9 * cache[i-5];
        val -= 27 * cache[i-6];
        val -= 6 * cache[i-7];
        cache[i] = val;
    }

    cache_n = new_n;
}

/* public entry point ------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* out of range – not used */
    ensure_cache(n);
    return cache[n-1];                    /* n is 1‑based, array is 0‑based */
}
