/*
 * A255432  Expansion of e.g.f. sqrt(1-x^2) * exp(x*(1+arctanh(x))).
 * https://oeis.org/A255432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + sum_{k=1..floor(n/2)} ((n-1)!/(n-2*k)!)/(2*k-1)
 *            * a(n-2*k), a(0)=1, a(1)=1. Recurrence: a(n) = a(n-1) +
 *            ∑_{k=1}^{⌊n/2⌋} binom(n-1, n-2k) * fact(2k-2) * a(n-2k)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    if (n >= cap) {
        size_t newcap = n + 1;
        cache = mem_resize(seq_t, cache, newcap);
        for (idx_t i = cap; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
                continue;
            }
            /* a(0) = 1 gives a(1) = a(0) */
            seq_t sum = cache[i - 1];
            idx_t maxk = i / 2;
            for (idx_t k = 1; k <= maxk; ++k) {
                seq_t term = binomial(i - 1, i - 2 * k);          /* C(i-1, i-2k)   */
                term = term * factorial(2 * k - 2);            /* * (2k-2)!       */
                term = term * cache[i - 2 * k];               /* * a(i-2k)       */
                sum   = sum   + term;
            }
            cache[i] = sum;
        }
        cap = newcap;
    }
    return cache[n];
}
