/*
 * A322366  Number of integers k in {0,1,...,n} such that k identical test
 *          tubes can be balanced in a centrifuge with n equally spaced holes.
 * https://oeis.org/A322366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = |{ k : k and n-k can be written as a sum of prime *
 *            factors of n }| (OEIS A322366)
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function performs a simple DP for each call; no memoisation is
 * required. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Count the number of k in [0,n] whose sum and the complementary sum
 * are representable using the prime factors of n. */
seq_t a(idx_t n)
{
    /* Special case: n==0 has one valid k=0. */
    if (n == 0) return (seq_t)1;

    /* --- 1. List unique prime divisors of n --------------------------------- */
    uint64_t primes[64];
    size_t   pcnt   = 0;
    uint64_t tmp    = (uint64_t)n;

    /* factor 2 separately */
    if ((tmp & 1ULL) == 0) {
        primes[pcnt++] = 2ULL;
        while ((tmp & 1ULL) == 0) tmp >>= 1;
    }

    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            primes[pcnt++] = p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1ULL) primes[pcnt++] = tmp;

    /* --- 2. DP array: rep[s] == true  <=>  s is representable ------ */
    size_t len = (size_t)n + 1;
    _Bool *rep = (_Bool *)malloc(sizeof(_Bool) * len);
    if (!rep) return (seq_t)0;               /* allocation failed */

    for (size_t i = 0; i < len; ++i) rep[i] = 0;
    rep[0] = 1;

    for (size_t ii = 0; ii < pcnt; ++ii) {
        uint64_t p = primes[ii];
        for (idx_t s = p; s <= n; ++s)
            if (rep[(size_t)(s - p)]) rep[(size_t)s] = 1;
    }

    /* --- 3. Count valid k -------------------------------------------------- */
    seq_t count = 0;
    for (idx_t k = 0; k <= n; ++k)
        if (rep[(size_t)k] && rep[(size_t)(n - k)]) ++count;

    free(rep);
    return count;
}
