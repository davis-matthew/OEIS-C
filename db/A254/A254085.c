/*
 * A254085  Number of (n+2)X(1+2) 0..1 arrays with every 3X3 subblock diagonal
 *          maximum minus antidiagonal maximum nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A254085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A254085:  Number of (n+2)×(1+2) 0..1 arrays with every 3×3 subblock
   diagonal‑max minus antidiagonal‑max nondecreasing horizontally,
   vertically and ne→sw antidiagonally.
   Empirical recurrence (valid for n > 17):
       a(n) =  6*a(n‑1) + 7*a(n‑2) – 18*a(n‑3) + 14*a(n‑4)
              –156*a(n‑5) –116*a(n‑6) + 72*a(n‑7)
              + 32*a(n‑8) + 96*a(n‑9) + 64*a(n‑10)
   Initial values:
       a(1)=512, a(2)=3436, a(3)=22728, a(4)=148004,
       a(5)=977192, a(6)=6446656, a(7)=42580424,
       a(8)=281186200, a(9)=1857372144,
       a(10)=12268544428, a(11)=81040372600,
       a(12)=535313103052, a(13)=3536037360504,
       a(14)=23357456516908, a(15)=154288871288536,
       a(16)=1019162914084204, a(17)=6732132852099864,
       a(18)=44469447874930540.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to store computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;

    /* Base values for 1..18 (index 0 unused) */
    static const seq_t base[19] = {
        0ULL, /* unused, keeps indices 1..18 aligned */
        512ULL, 3436ULL, 22728ULL, 148004ULL,
        977192ULL, 6446656ULL, 42580424ULL,
        281186200ULL, 1857372144ULL,
        12268544428ULL, 81040372600ULL,
        535313103052ULL, 3536037360504ULL,
        23357456516908ULL, 154288871288536ULL,
        1019162914084204ULL, 6732132852099864ULL,
        44469447874930540ULL
    };

    /* Initialisation on first call */
    if (!cache) {
        cache_max = 18;
        cache = (seq_t*)malloc((cache_max + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* allocation failed */
        for (idx_t i = 0; i <= cache_max; ++i)
            cache[i] = base[i];
    }

    /* Return if already computed */
    if (n <= cache_max)
        return cache[n];

    /* Grow cache to accommodate n */
    seq_t *new_cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;           /* allocation failed */
    cache = new_cache;

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* Sum of positive coefficients */
        seq_t sum = 0;
        sum += (seq_t)6  * cache[i - 1];
        sum += (seq_t)7  * cache[i - 2];
        sum += (seq_t)14 * cache[i - 4];
        sum += (seq_t)32 * cache[i - 8];
        sum += (seq_t)96 * cache[i - 9];
        sum += (seq_t)64 * cache[i -10];
        sum += (seq_t)72 * cache[i - 7];

        /* Sum of negative coefficients */
        seq_t diff = 0;
        diff += (seq_t)18  * cache[i - 3];
        diff += (seq_t)156 * cache[i - 5];
        diff += (seq_t)116 * cache[i - 6];

        cache[i] = sum - diff;   /* result is guaranteed non‑negative */
    }

    cache_max = n;
    return cache[n];
}
