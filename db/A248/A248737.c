/*
 * A248737  a(0) = 0; a(n) = smallest k such that gcd(a(n-k), n) is not 1.
 * https://oeis.org/A248737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; a(1)=1; for n>1 a(n) = smallest k (1 ≤ k ≤ n) such that gcd(a(n‑k), n) ≠ 1. */
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t len = 0;  /* number of pre‑computed terms (cache[0 .. len-1]) */

    /* Ensure capacity for terms up to n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : (size_t)n + 1;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {  /* allocation failed */
            abort();
        }
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = 0;
        } else if (i == 1) {
            cache[i] = 1;
        } else {
            seq_t n_seq = (seq_t)i;  /* cast index to seq_t for gcd argument */
            seq_t result = 0;
            for (idx_t k = 1; k <= i; ++k) {
                if (gcd(cache[i - k], n_seq) != 1) {
                    result = (seq_t)k;
                    break;
                }
            }
            cache[i] = result;
        }
    }
    len = n + 1;
    return cache[n];
}
