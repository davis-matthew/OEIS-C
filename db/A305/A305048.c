/*
 * A305048  Number of ordered pairs (k, m) of nonnegative integers such that
 *          5^k + 10^m is not only a primitive root modulo prime(n) but also
 *          smaller than prime(n).
 * https://oeis.org/A305048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 2169, u32 n <= 8888, u64 n <= 8888,
 *            u128 n <= 8888, big any n
 * Helpers:   factor_u64, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A305048: For the n‑th prime p, count ordered pairs (k,m) of
 * non‑negative integers such that g = 5^k + 10^m satisfies
 * g < p and g is a primitive root modulo p. */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/mod_pow.h"

/* Return the number of admissible pairs for the n‑th prime. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* n‑th prime (1‑based indexing) */
    seq_t p = get_prime(n);

    /* p-1 is small enough for 64‑bit arithmetic (valid for the
     * intended range n ≤ 10 000). */
    uint64_t p_minus1 = (uint64_t)p - 1;

    /* Factor p-1 into distinct prime factors. */
    uint64_t prime_factors[64], exponents[64];
    size_t factor_cnt = 0;
    factor_u64(p_minus1, prime_factors, exponents, &factor_cnt);

    seq_t count = 0;
    seq_t pow5 = 1;        /* current 5^k */

    while (pow5 < p) {
        seq_t pow10 = 1;  /* current 10^m */
        while (pow5 + pow10 < p) {
            seq_t g = pow5 + pow10;

            /* Check if g is a primitive root modulo p. */
            bool ok = true;
            for (size_t i = 0; i < factor_cnt && ok; ++i) {
                uint64_t q   = prime_factors[i];
                uint64_t exp = p_minus1 / q;
                seq_t     res = mod_pow(g, exp, p);
                if (res == (seq_t)1) ok = false;
            }
            if (ok) ++count;

            pow10 = pow10 * (seq_t)10;
        }
        pow5 = pow5 * (seq_t)5;
    }

    return count;
}
