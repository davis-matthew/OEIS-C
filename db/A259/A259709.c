/*
 * A259709  a(n) = A259708(n,n-1).
 * https://oeis.org/A259709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 6*a(n-3) + 2*a(n-4) + 4*a(n-5) +
 *            a(n-6) Initial values: a(1)=0, a(2)=-1, a(3)=0, a(4)=4, a(5)=22,
 *            a(6)=78
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 24, i64 n <= 49, i128 n <= 100,
 *            big any n
 * Helpers:   grow_seq, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/grow_seq.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* computed terms */
    static size_t computed  = 0;       /* number of terms computed */
    static size_t cache_cap = 0;       /* current capacity of cache */

    if (n == 0) return 0;             /* not part of the sequence, harmless */

    size_t idx = (size_t)n;           /* zero‑based array index is n-1 */

    /* Ensure we have storage for terms up to n */
    grow_seq(&cache, &cache_cap, idx);

    /* Compute missing terms */
    for (size_t i = computed; i < idx; ++i) {
        if (i == 0)         cache[i] = (seq_t)0;          /* a(1) */
        else if (i == 1)    cache[i] = (seq_t)-1;         /* a(2) */
        else if (i == 2)    cache[i] = (seq_t)0;          /* a(3) */
        else if (i == 3)    cache[i] = (seq_t)4;          /* a(4) */
        else if (i == 4)    cache[i] = (seq_t)22;         /* a(5) */
        else if (i == 5)    cache[i] = (seq_t)78;         /* a(6) */
        else {
            seq_t t;
            t = mul_small(cache[i-1], 4);
            t -= mul_small(cache[i-2], 2);
            t -= mul_small(cache[i-3], 6);
            t += mul_small(cache[i-4], 2);
            t += mul_small(cache[i-5], 4);
            t += cache[i-6];
            cache[i] = t;
        }
    }
    computed = idx;
    return cache[n-1];
}
