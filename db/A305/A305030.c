/*
 * A305030  Number of pairs {k, m} with 0 <= k <= m such that binomial(2k,k) +
 *          binomial(2m,m) is not only a primitive root modulo prime(n) but
 *          also smaller than prime(n).
 * https://oeis.org/A305030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs {k,m} (0 ≤ k ≤ m) such that *
 *            binomial(2k,k) + binomial(2m,m) is a primitive root modulo the *
 *            n‑th prime and the sum is < that prime. The function does a *
 *            direct search over all central binomial coefficients < prime
 * Range:     u8 n <= 8, u16 n <= 1098, u32 n <= 9532, u64 n <= 9532,
 *            u128 n <= 9532, big any n
 * Helpers:   binomial, cmp_seq_t, factorize, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/get_prime.h"     /* uint32_t get_prime(idx_t k); */
#include "../utils/binomial.h"      /* seq_t binomial(idx_t n, idx_t k); */
#include "../utils/mod_pow.h"       /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */
#include "../utils/factorize.h"     /* int factorize(uint64_t n, uint64_t primes[], int exps[]); */
#include "../utils/cmp_seq_t.h"     /* int cmp_seq_t(const void *p1,const void *p2); */

static bool is_primitive_root(seq_t g, seq_t p, const uint64_t *factors, int cnt)
{
    uint64_t p_val = (uint64_t)p;          /* p is <= 2^32 from get_prime */
    for (int i = 0; i < cnt; ++i) {
        uint64_t q   = factors[i];
        uint64_t exp = (p_val - 1) / q;
        if (mod_pow(g, exp, p) == (seq_t)1)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)                       /* offset is 1 – a(1) corresponds to prime(1)=2 */
        return (seq_t)0;

    /* Get the n‑th prime.  It fits in 32 bits. */
    uint32_t p32 = get_prime(n);
    seq_t     p  = (seq_t)p32;

    /* Build an array of all central binomial coefficients < p. */
    seq_t cb[64];
    size_t cb_len = 0;
    for (idx_t k = 0; ; ++k) {
        seq_t val = binomial(2 * k, k);
        if (cmp_seq_t(&val, &p) >= 0)     /* val >= p, stop */
            break;
        if (cb_len < 64)
            cb[cb_len++] = val;
        else
            break;                      /* safety, should never hit 64 for 32‑bit primes */
    }

    /* Factorise p‑1.  Only the distinct prime factors are needed. */
    uint64_t prime_factors[32];
    int      exps[32];
    int      factor_cnt = factorize((uint64_t)p32 - 1, prime_factors, exps);
    if (factor_cnt > 32)          /* safety, should not happen for 32‑bit primes */
        factor_cnt = 32;

    /* Count qualifying pairs. */
    uint64_t count = 0;
    for (size_t i = 0; i < cb_len; ++i) {
        uint64_t bk = (uint64_t)cb[i];   /* cb[i] < p32 ≤ 2^32, fits in 64 bits */
        for (size_t j = i; j < cb_len; ++j) {
            uint64_t sum64 = bk + (uint64_t)cb[j];
            if (sum64 >= p32)            /* sums increase with j */
                break;
            seq_t sum_val = (seq_t)sum64;
            if (is_primitive_root(sum_val, p, prime_factors, factor_cnt))
                ++count;
        }
    }

    return (seq_t)count;
}
