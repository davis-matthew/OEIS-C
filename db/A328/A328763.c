/*
 * A328763  a(n) = A328613(A276086(n)).
 * https://oeis.org/A328763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A328613(A276086(n)); A276086(n) = Product_k
 *            prime(k+1)^d_k(n) with d_k the primorial-base digits of n, then
 *            A328613 iterates x -> floor(x/p) over successive primes
 * Range:     u8 n <= 8, u16 n <= 28, u32 n <= 178, u64 n <= 1858,
 *            u128 n <= 2559, big any n
 * Validated: standard, pass, 91 terms through n = 2516 (2026-11-08)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* A276086(n): digits of n in primorial base become prime exponents */
    seq_t x = 1;
    {
        idx_t r = n;
        uint64_t q = 2;
        while (r > 0) {
            idx_t d = r % q;
            r /= q;
            for (idx_t i = 0; i < d; ++i)
                x *= (seq_t)q;
            q = next_prime(q);
        }
    }

    /* A328613(x) */
    uint64_t p = 2;
    seq_t m = 1;
    while (x > 0) {
        seq_t t = x;
        while (t % p == 0) {
            t /= p;
            m *= (seq_t)p;
        }
        x /= p;
        p = next_prime(p);
    }
    return m * (seq_t)p;
}
