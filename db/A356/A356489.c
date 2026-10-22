/*
 * A356489  a(n) = A000265(rad(prime(n)-1)), rad = A007947.
 * https://oeis.org/A356489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{odd primes p dividing prime(n)-1} p, for n ≥ 1
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)                   /* undefined for offset < 1, return 0 as placeholder */
        return (seq_t)0;

    /* Fetch the n‑th prime (1‑based: get_prime(1) == 2). */
    uint32_t p = get_prime(n);
    uint64_t m = (uint64_t)p - 1ULL;       /* m = prime(n) - 1 */

    /* Factor m into distinct primes. */
    uint64_t fac[64];                   /* prime factors */
    uint64_t exps[64];                  /* exponents (unused) */
    size_t   cnt = 0;                   /* number of distinct factors */

    factor_u64(m, fac, exps, &cnt);

    /* Multiply all odd primes found. */
    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        if (fac[i] != 2ULL)
            prod *= (seq_t)fac[i];
    }
    return prod;
}
