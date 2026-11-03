/*
 * A389501  Numbers whose coreful divisors have a noninteger arithmetic mean.
 * https://oeis.org/A389501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth positive integer k such that the arithmetic *
 *            mean of the coreful divisors of k is non‑integer. * This is
 *            found by scanning integers until the n-th qualifying number * is
 *            reached
 * Range:     u8 n <= 25, u16 n <= 6203, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9531 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* test whether k has a non‑integer arithmetic mean of its coreful divisors */
static bool coreful_mean_noninteger(uint64_t k)
{
    if (k < 2) return false;

    uint64_t tmp = k;
    uint64_t primes[32];
    uint32_t  exps[32];
    int nf = 0;
    bool has_nonunit = false;

    /* factorisation */
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint32_t e = 0;
            do {
                tmp /= p;
                ++e;
            } while (tmp % p == 0);
            primes[nf] = p;
            exps[nf] = e;
            ++nf;
            if (e > 1) has_nonunit = true;
        }
    }
    if (tmp > 1) {               /* remaining prime factor */
        primes[nf] = tmp;
        exps[nf] = 1;
        ++nf;
    }
    if (!has_nonunit) return false;   /* square‑free numbers are excluded */

    /* number of coreful divisors = product of the prime exponents */
    uint64_t count = 1;
    for (int i = 0; i < nf; ++i) count *= (uint64_t)exps[i];
    if (count <= 1) return false;

    /* compute sum modulo count */
    uint64_t sum_mod = 1 % count;   /* start with 1 for multiplicative product */
    for (int i = 0; i < nf; ++i) {
        uint64_t p   = primes[i];
        uint32_t e   = exps[i];
        unsigned __int128 pow = 1;
        for (uint32_t j = 0; j < e; ++j) pow *= (unsigned __int128)p;

        unsigned __int128 num   = (unsigned __int128)p * (pow - 1);
        unsigned __int128 denom = p - 1;
        unsigned __int128 sum_i = num / denom;

        uint64_t sum_i_mod = (uint64_t)(sum_i % count);
        sum_mod = (uint64_t)(((__uint128_t)sum_mod * sum_i_mod) % count);
    }
    return sum_mod != 0;   /* non‑integer mean iff sum_mod != 0 */
}

/* external entry point as required by the build system */
extern seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n = 0, but safe */

    uint64_t target  = (uint64_t)n;
    uint64_t found   = 0;
    uint64_t k       = 2;        /* start scanning from 2 */

    while (found < target) {
        if (coreful_mean_noninteger(k)) {
            ++found;
            if (found == target) return (seq_t)k;
        }
        ++k;
    }

    return (seq_t)0;   /* should never be reached */
}
