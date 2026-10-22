/*
 * A393808  a(n) = Sum_{k=1..n} gcd(k^4,n).
 * https://oeis.org/A393808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{k=1..n} gcd(k^4, n)
 * Range:     u8 n <= 26, u16 n <= 624, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static void factor_uint64(uint64_t n, uint64_t *primes, uint64_t *exps, size_t *cnt) {
    size_t c = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[c] = p;
            exps[c] = e;
            ++c;
        }
    }
    if (n > 1) {
        primes[c] = n;
        exps[c] = 1;
        ++c;
    }
    *cnt = c;
}

static seq_t prime_power_contribution(idx_t p, idx_t e) {
    /* Compute a(p^e) = p^e + Σ_{k=0}^{e-1} (p^{e-k} - p^{e-k-1}) · p^{min(4k, e)} */
    seq_t p_pow[65];
    p_pow[0] = 1;
    for (idx_t i = 1; i <= e; ++i) {
        p_pow[i] = p_pow[i - 1] * (seq_t)p;
    }
    seq_t res = p_pow[e];
    for (idx_t k = 0; k < e; ++k) {
        idx_t exp1 = e - k;
        idx_t exp2 = exp1 - 1;
        seq_t term1 = p_pow[exp1] - p_pow[exp2];
        idx_t min_exp = 4 * k;
        if (min_exp > e) min_exp = e;
        seq_t term2 = p_pow[min_exp];
        res += term1 * term2;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 1) return 1;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_uint64((uint64_t)n, primes, exps, &cnt);
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = prime_power_contribution(primes[i], exps[i]);
        result *= term;
    }
    return result;
}
