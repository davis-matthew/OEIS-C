/*
 * A246886  Number of length n+4 0..2 arrays with some pair in every
 *          consecutive five terms totalling exactly 2.
 * https://oeis.org/A246886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+2*a(n-2)+a(n-3)+3*a(n-4)
 *            -2*a(n-6)-2*a(n-7)-3*a(n-8)-3*a(n-9) +a(n-11)+a(n-13)+a(n-14)
 *            with initial terms a(1)=231, a(2)=673, a(3)=1961, a(4)=5711,
 *            a(5)=16621, a(6)=48393, a(7)=140893, a(8)=410197, a(9)=1194243,
 *            a(10)=3476929, a(11)=10122747, a(12)=29471409, a(13)=85803183,
 *            a(14)=...
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 16, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static const seq_t init_vals[14] = {
    (seq_t)231,
    (seq_t)673,
    (seq_t)1961,
    (seq_t)5711,
    (seq_t)16621,
    (seq_t)48393,
    (seq_t)140893,
    (seq_t)410197,
    (seq_t)1194243,
    (seq_t)3476929,
    (seq_t)10122747,
    (seq_t)29471409,
    (seq_t)85803183,
    (seq_t)249807769
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* offset handling */
    if (n <= 14) return init_vals[n-1];

    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;              /* allocated capacity   */
    static idx_t computed = 0;               /* number of values computed (>=14) */

    /* allocate or grow cache */
    if (!cache) {
        cache_cap = (n > 14 ? n : 14);
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) abort();
        memcpy(cache, init_vals, sizeof(init_vals));
        computed = 14;
    } else if (n > cache_cap) {
        cache_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
        if (!tmp) abort();
        cache = tmp;
    }

    /* compute values up to n */
    for (idx_t i = computed; i < n; ++i) {
        /* i is zero‑based index for a(i+1) */
        seq_t pos = 2*cache[i-1]  + 2*cache[i-2]  + cache[i-3]
                  + 3*cache[i-4];
        pos += cache[i-11] + cache[i-13] + cache[i-14];
        seq_t neg = 2*cache[i-6]  + 2*cache[i-7]  + 3*cache[i-8]
                  + 3*cache[i-9];
        cache[i] = pos - neg;     /* pos >= neg for all n >= 15 */
    }
    computed = n;
    return cache[n-1];
}
