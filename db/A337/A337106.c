/*
 * A337106  Number of nontrivial divisors of n!.
 * https://oeis.org/A337106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = τ(n!) - 2 (τ(m) = number of divisors of m); a(0)=a(1)=0
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of non‑trivial divisors of n! for any idx_t n ≥ 0.
   Works for 64‑bit and arbitrary‑precision seq_t. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* For n < 2 the answer is defined as 0. */
    if (n < 2)
        return (seq_t)0;

    /* ---------- Prime sieve up to n ---------- */
    size_t lim = (size_t)n;
    uint8_t *sieve = (uint8_t *)calloc(lim + 1, sizeof(uint8_t));
    if (!sieve) return (seq_t)0;                /* allocation failed */

    size_t primes_cap = lim / 2 + 1;
    uint64_t *primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    if (!primes) { free(sieve); return (seq_t)0; }

    size_t primes_cnt = 0;
    for (size_t i = 2; i <= lim; ++i) {
        if (!sieve[i]) {
            primes[primes_cnt++] = (uint64_t)i;
            if (i * i <= lim)
                for (size_t j = i * i; j <= lim; j += i)
                    sieve[j] = 1;
        }
    }
    free(sieve);

    /* ---------- Compute τ(n!) via prime exponents ---------- */
    seq_t tau = (seq_t)1;
    for (size_t ip = 0; ip < primes_cnt; ++ip) {
        idx_t p = (idx_t)primes[ip];
        idx_t e = 0;
        idx_t pp = p;
        while (pp <= n) {
            e += n / pp;
            if (pp > n / p) break;    /* prevent overflow of pp * p */
            pp *= p;
        }
        tau = tau * (seq_t)(e + 1);
    }

    free(primes);
    return tau - (seq_t)2;   /* exclude divisor 1 and n! itself */
}
