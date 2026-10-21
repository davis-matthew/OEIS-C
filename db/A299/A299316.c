/*
 * A299316  Number of nX3 0..1 arrays with every element equal to 0, 3, 4, 5,
 *          6 or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n‑1) + 5*a(n‑2) - 5*a(n‑3) - 10*a(n‑4) + 4*a(n‑5) +
 *            a(n‑6) + 3*a(n‑7) - 3*a(n‑8) + 4*a(n‑9) - 2*a(n‑10) Initial
 *            values: a(1)=1, a(2)=1, a(3)=15, a(4)=10, a(5)=70, a(6)=146,
 *            a(7)=434, a(8)=1206, a(9)=3228, a(10)=9186, a(11)=24772,
 *            a(12)=69398
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache     = NULL;
static idx_t  cached_max = 0;   /* highest n that has been computed */
static size_t cap       = 0;    /* allocated size of cache (≥ cached_max+1) */

static void grow_cache(idx_t need)
{
    /* Ensure cache has space for indices 0 … need   (index 0 unused) */
    if (need + 1 <= cap) return;
    size_t new_cap = need + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                 /* allocation failure */
    cache = tmp;
    cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n=0, return 0 as guard */

    /* initialise cache with known values if not yet done */
    if (cached_max < 12) {
        grow_cache(12);
        cache[1]  = (seq_t)1;
        cache[2]  = (seq_t)1;
        cache[3]  = (seq_t)15;
        cache[4]  = (seq_t)10;
        cache[5]  = (seq_t)70;
        cache[6]  = (seq_t)146;
        cache[7]  = (seq_t)434;
        cache[8]  = (seq_t)1206;
        cache[9]  = (seq_t)3228;
        cache[10] = (seq_t)9186;
        cache[11] = (seq_t)24772;
        cache[12] = (seq_t)69398;
        cached_max = 12;
    }

    /* compute any missing terms up to n */
    if (n > cached_max) {
        grow_cache(n);
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            if (i <= 12) {
                /* values 1..12 already set, skip */
                continue;
            }
            /* compute using the recurrence with positive terms first */
            seq_t pos = 0;
            pos += (seq_t)2 * cache[i-1];
            pos += (seq_t)5 * cache[i-2];
            pos += (seq_t)4 * cache[i-5];
            pos +=           cache[i-6];
            pos += (seq_t)3 * cache[i-7];
            pos += (seq_t)4 * cache[i-9];

            seq_t neg = 0;
            neg += (seq_t)5 * cache[i-3];
            neg += (seq_t)10 * cache[i-4];
            neg += (seq_t)3 * cache[i-8];
            neg += (seq_t)2 * cache[i-10];

            cache[i] = pos - neg;
        }
        cached_max = n;
    }

    return cache[n];
}
