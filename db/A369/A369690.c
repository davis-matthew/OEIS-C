/*
 * A369690  a(n) = max(A119288(n), A053669(n)).
 * https://oeis.org/A369690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max( second smallest prime factor of n * (or 1 if n is a
 *            prime power), * smallest prime that does not divide n )
 * Range:     u8 n <= 513, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factorize, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factorize.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* Find the second smallest prime factor (p1). */
    uint64_t fac[20];
    int  exp[20];
    size_t faccnt = factorize(n, fac, exp);      /* fac[0] < fac[1] < ... */
    seq_t p1 = (faccnt >= 2 ? (seq_t)fac[1] : (seq_t)1);

    /* Find the smallest prime not dividing n (p2). */
    seq_t p2 = 0;
    for (seq_t cand = 2; ; ++cand) {
        if (!is_prime_big(cand))
            continue;        /* skip non‑primes */
        /* cand fits into idx_t (n is 64‑bit), so modulo is safe. */
        if (n % (idx_t)cand != 0) {
            p2 = cand;
            break;
        }
    }

    return (p1 > p2) ? p1 : p2;
}
