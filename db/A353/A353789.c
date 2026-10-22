/*
 * A353789  Multiplicative with a(p^e) = (q - 1) * q^(e-1) * p^e, where q is
 *          the least prime greater than p.
 * https://oeis.org/A353789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e||n} (q - 1) * q^{e-1} * p^e, where q =
 *            nextprime(p) (the least prime > p). This computes the
 *            multiplicative function A353789
 * Range:     u8 n <= 11, u16 n <= 127, u32 n <= 8191, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, next_prime, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition */
    if (n == 1)
        return (seq_t)1;

    /* Preallocate buffers for factorization: enough for all 64‑bit ints. */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;

    /* Factor n into primes and exponents. */
    factor_u64(n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t q = next_prime(p);

        seq_t p_pow = seq_pow((seq_t)p, (idx_t)e);
        seq_t q_pow = (e > 1 ? seq_pow((seq_t)q, (idx_t)(e - 1)) : (seq_t)1);

        /* a(p^e) = (q - 1) * q^(e-1) * p^e. */
        seq_t term = p_pow * q_pow;
        term = term * (q - 1);

        result *= term;
    }

    return result;
}
