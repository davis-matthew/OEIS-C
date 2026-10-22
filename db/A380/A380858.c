/*
 * A380858  a(n) is the number of primes p <= n such that p^(p + n) == p (mod
 *          p + n).
 * https://oeis.org/A380858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = |{ primes p ≤ n | p^(p+n) ≡ p (mod p+n) }|
 * Range:     u8 n <= 290, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mod_pow.h"   /* provides mod_pow(seq_t, uint64_t, seq_t) */

seq_t a(idx_t n)
{
    if (n < 2) /* no primes ≤ n */
        return 0;

    /* Simple sieve of Eratosthenes up to n */
    bool *is_prime = (bool*)malloc((size_t)(n + 1));
    if (!is_prime) return 0;
    for (idx_t i = 0; i <= n; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (idx_t i = 2; i * i <= n; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= n; j += i)
                is_prime[j] = false;
        }
    }

    seq_t count = 0;
    for (idx_t p = 2; p <= n; ++p) {
        if (is_prime[p]) {
            seq_t mod = (seq_t)(p + n);
            seq_t res = mod_pow((seq_t)p, (uint64_t)(p + n), mod);
            if (res == (seq_t)p)
                ++count;
        }
    }

    free(is_prime);
    return count;
}
