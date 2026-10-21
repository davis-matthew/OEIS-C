/*
 * A243282  Partial sums of the characteristic function for A070003.
 * https://oeis.org/A243282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k ≤ n whose largest prime factor p satisfies p²
 *            | k (k ≥ 2)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/largest_prime_divisor.h"

seq_t a(idx_t n) {
    /* static cache of partial sums */
    static seq_t *cum = NULL;
    static idx_t capacity = 0;
    static idx_t max_comp = 1;

    if (n < 1) return 0;              /* offset is 1, a(1)=0 */

    /* initialise cache on first call */
    if (!cum) {
        capacity = 2;                 /* at least indices 0 .. 1 */
        cum = (seq_t*)calloc(capacity, sizeof(seq_t));
        if (!cum) return 0;           /* allocation failure → 0 (not expected) */
        cum[0] = 0;                   /* not used but keeps indices aligned */
        cum[1] = 0;                   /* a(1) = 0 */
        max_comp = 1;
    }

    /* extend capacity if necessary */
    if (n >= capacity) {
        while (capacity <= n) capacity <<= 1;  /* double until enough */
        seq_t *tmp = (seq_t*)realloc(cum, capacity * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failure → 0 (not expected) */
        cum = tmp;
    }

    /* compute new terms up to n */
    for (idx_t k = max_comp + 1; k <= n; ++k) {
        uint64_t kk = (uint64_t)k;
        uint64_t p  = largest_prime_divisor(kk);
        /* check if p^2 divides k without risking overflow:
           k is divisible by p; check if k/p is still divisible by p */
        seq_t good = ((kk / p) % p == 0) ? 1 : 0;
        cum[k] = cum[k - 1] + good;
    }
    max_comp = n;
    return cum[n];
}
