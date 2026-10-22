/*
 * A318871  Minimum Heinz number of a factorization of n into factors > 1.
 * https://oeis.org/A318871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 543, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, int_sqrt, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimum Heinz number:
   a(1) = 1
   for n > 1:
       a(n) = min_{d|n, d>1} prime(d) * a(n/d)
   (prime(k) = k‑th prime) */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k); */
#include "../utils/int_sqrt.h"   /* idx_t int_sqrt(idx_t n); */
#include "../utils/seq_mul.h"    /* seq_t seq_mul(seq_t a, seq_t b); */

/* Recursive computation of the minimal Heinz number. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Start with the factorization consisting of n itself: prime(n)*a(1). */
    seq_t best = (seq_t)get_prime(n);

    idx_t r = int_sqrt(n);
    for (idx_t d = 2; d <= r; ++d) {
        if (n % d == 0) {
            /* Use divisor d. */
            seq_t sub = a(n / d);
            seq_t cand = seq_mul((seq_t)get_prime(d), sub);
            if (cand < best)
                best = cand;

            /* Use the complementary divisor e = n/d if distinct from d. */
            idx_t e = n / d;
            if (e != d) {
                sub = a(e);
                cand = seq_mul((seq_t)get_prime(e), sub);
                if (cand < best)
                    best = cand;
            }
        }
    }
    return best;
}
