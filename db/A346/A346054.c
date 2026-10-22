/*
 * A346054  Number of ways to tile a 3 X n strip with dominoes and L-shaped
 *          5-minoes.
 * https://oeis.org/A346054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + 5*a(n-3) - 4*a(n-4) (a(0)=1, a(1)=0,
 *            a(2)=3, a(3)=8)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last = (idx_t)-1;

    if (cache == NULL) {
        cap   = 4;
        cache = seq_alloc(cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)8;
        last = 3;
    }

    if (n > last) {
        if ((idx_t)cap <= n) {
            size_t newcap = (size_t)(n + 1);
            cache = seq_realloc(cache, cap, newcap);
            cap   = newcap;
        }
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t a4 = cache[i - 4];
            cache[i] = a1 + (seq_t)3 * a2 + (seq_t)5 * a3 - (seq_t)4 * a4;
        }
        last = n;
    }

    return cache[n];
}
