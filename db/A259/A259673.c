/*
 * A259673  a(n) = sigma_(prime(n))(n).
 * https://oeis.org/A259673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma_k(n) where k = prime(n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow, factor_u64, get_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*      sigma_k(n) = sum_{d|n} d^k */

/* This function computes the sum of the k‑th power of all divisors of n,
 * where k is the n‑th prime.  It uses utility functions to factor n,
 * get the n‑th prime, and to compute large exponentiations and
 * multiplications on the arbitrary‑precision type seq_t. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"     /* uint32_t get_prime(idx_t) */
#include "../utils/factor_u64.h"    /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/big_pow.h"       /* seq_t big_pow(seq_t base, seq_t exp) */
#include "../utils/seq_mul.h"       /* seq_t seq_mul(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    /* n == 0 is undefined in the OEIS sequence; return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    /* The exponent k is the nth prime (offset 1). */
    uint32_t pk_int = get_prime(n);
    seq_t k = (seq_t)pk_int;

    /* Factor n into primes.  n fits into uint64_t, so we use factor_u64. */
    size_t cnt = 0;
    /* 64 is plenty for the number of distinct prime factors of any n <= 2^64. */
    uint64_t pr[64], ex[64];
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* Compute the product over each prime factor of the geometric series. */
    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        idx_t e   = (idx_t)ex[i];      /* exponent of the prime factor */

        /* p^k once, then reuse for higher powers */
        seq_t pk = big_pow((seq_t)p, k);

        seq_t sum  = (seq_t)1;         /* sum of 1 + p^k + ... */
        seq_t term = (seq_t)1;         /* current term p^{j*k} */

        for (idx_t j = 1; j <= e; ++j) {
            term = seq_mul(term, pk);  /* term = term * p^k = p^{j*k} */
            sum += term;
        }

        result = seq_mul(result, sum); /* result *= sum */
    }

    return result;
}
