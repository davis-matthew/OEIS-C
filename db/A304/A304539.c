/*
 * A304539  Number of nX3 0..1 arrays with every element unequal to 0, 1, 3, 4
 *          or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) + 2*a(n-3) - 8*a(n-4) + 2*a(n-5) -
 *            a(n-6) - a(n-7) - 9*a(n-8) + 3*a(n-9) + 9*a(n-10) + 8*a(n-11) +
 *            a(n-12) - 3*a(n-13) Initial values: a(1)=3, a(2)=7, a(3)=9,
 *            a(4)=19, a(5)=37, a(6)=65, a(7)=129, a(8)=275, a(9)=555,
 *            a(10)=1143, a(11)=2417, a(12)=5101, a(13)=10937. The seq...
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 28, u64 n <= 54, u128 n <= 106,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;   /* array of a(i) with 0‑based index: cache[0]=a(1) */
    static idx_t size = 0;        /* number of terms currently stored (≥ 13 after init) */
    static idx_t capacity = 0;    /* allocated capacity of cache */

    /* initialise cache with the first 13 terms on the first call */
    if (size == 0) {
        const seq_t init[13] = {
            (seq_t)3,   (seq_t)7,   (seq_t)9,   (seq_t)19,
            (seq_t)37,  (seq_t)65,  (seq_t)129, (seq_t)275,
            (seq_t)555, (seq_t)1143, (seq_t)2417, (seq_t)5101,
            (seq_t)10937
        };
        ensure_capacity(13, &capacity, (void **)&cache, sizeof(seq_t));
        for (idx_t i = 0; i < 13; ++i) cache[i] = init[i];
        size = 13;
    }

    /* trivial cases */
    if (n <= size) return cache[n - 1];

    /* extend the cache up to index n-1 (a(n)) */
    ensure_capacity((size_t)n, &capacity, (void **)&cache, sizeof(seq_t));

    for (idx_t i = size; i < n; ++i) {
        /* i is 0‑based: cache[i] will store a(i+1) */
        /* Compute positive contributions */
        seq_t pos = (seq_t)3 * cache[i - 1];
        pos += (seq_t)2 * cache[i - 3];
        pos += (seq_t)2 * cache[i - 5];
        pos += (seq_t)3 * cache[i - 9];
        pos += (seq_t)9 * cache[i - 10];
        pos += (seq_t)8 * cache[i - 11];
        pos += cache[i - 12];

        /* Compute negative contributions */
        seq_t neg = cache[i - 2];
        neg += (seq_t)8 * cache[i - 4];
        neg += cache[i - 6];
        neg += cache[i - 7];
        neg += (seq_t)9 * cache[i - 8];
        neg += (seq_t)3 * cache[i - 13];

        /* final value: pos - neg (pos >= neg for all n) */
        cache[i] = pos - neg;
    }
    size = n;
    return cache[n - 1];
}
