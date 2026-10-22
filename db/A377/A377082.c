/*
 * A377082  a(0) = 1, a(1) = 3, a(2) = 13; for n >= 2, a(n+1) =
 *          2*3^((a(n)-1)/4) - 1.
 * https://oeis.org/A377082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 3, a(2) = 13, a(n+1) = 2*3^((a(n)-1)/4) - 1 for
 *            n >= 2
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap = 0;
    static idx_t size = 0;
    if (n >= cap) {
        idx_t new_cap = cap ? cap * 2 : 1;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cap = new_cap;
    }
    if (size <= n) {
        for (idx_t i = size; i <= n; ++i) {
            if (i == 0) cache[i] = (seq_t)1;
            else if (i == 1) cache[i] = (seq_t)3;
            else if (i == 2) cache[i] = (seq_t)13;
            else {
                seq_t prev = cache[i-1];
                seq_t exp_t = (prev - (seq_t)1) / (seq_t)4;
                idx_t exp = (idx_t)exp_t;
                seq_t val = pow3(exp);
                val = val * (seq_t)2;
                val = val - (seq_t)1;
                cache[i] = val;
            }
        }
        size = n + 1;
    }
    return cache[n];
}
