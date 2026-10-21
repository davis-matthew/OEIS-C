/*
 * A275463  Least k such that n divides d(k*n) (d = A000005).
 * https://oeis.org/A275463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 36, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   big_pow, factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A275463: a(n) = least k ≥ 1 such that n | τ(k·n) (τ = divisor function) */
/* For prime n > 3, a(n) = 2^(n-1); otherwise brute‑force n | τ(k·n). */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

/* ---------------------------------------------------------- */
/*  returns a(n) for all n ≥ 1                                 */
/* ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* a(1) = 1 */
    if (n == 1)
        return (seq_t)1;

    /* Prime numbers have a closed form */
    if (is_prime(n)) {
        if (n == 2)
            return (seq_t)1;           /* a(2) = 1 */
        if (n == 3)
            return (seq_t)3;           /* a(3) = 3 */
        /* a(p) = 2^(p-1) for p > 3 */
        return big_pow((seq_t)2, (seq_t)(n - 1));
    }

    /* ----- composite n: trial k ----- */
    /* factor n once */
    uint64_t primes_n[32], exponents_n[32];
    size_t cnt_n = 0;
    factor_u64((uint64_t)n, primes_n, exponents_n, &cnt_n);

    /* iterate k = 1,2,3,... */
    for (idx_t k = 1; ; ++k) {
        /* factor k */
        uint64_t primes_k[32], exponents_k[32];
        size_t cnt_k = 0;
        factor_u64(k, primes_k, exponents_k, &cnt_k);

        /* combine prime lists and compute τ(k·n) */
        uint64_t all_primes[64];
        uint64_t all_exponents[64];
        size_t cnt_all = 0;

        /* copy n's factors */
        for (size_t i = 0; i < cnt_n; ++i) {
            all_primes[cnt_all]     = primes_n[i];
            all_exponents[cnt_all]  = exponents_n[i];
            ++cnt_all;
        }

        /* merge k's factors */
        for (size_t i = 0; i < cnt_k; ++i) {
            uint64_t p = primes_k[i];
            uint64_t e = exponents_k[i];
            bool found = false;
            for (size_t j = 0; j < cnt_all; ++j) {
                if (all_primes[j] == p) {
                    all_exponents[j] += e;
                    found = true;
                    break;
                }
            }
            if (!found) {
                all_primes[cnt_all]     = p;
                all_exponents[cnt_all]  = e;
                ++cnt_all;
            }
        }

        /* compute τ(k·n) = Π (exponent+1) */
        seq_t d = (seq_t)1;
        for (size_t i = 0; i < cnt_all; ++i)
            d *= (seq_t)(all_exponents[i] + 1);

        /* check divisibility */
        if ((d % (seq_t)n) == (seq_t)0)
            return (seq_t)k;
    }
}
