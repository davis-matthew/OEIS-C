/*
 * A263827  The number c_{Cc pi_1(B_1)}(2n) of the second amphicosm
 *          2n-coverings over the first amphicosm.
 * https://oeis.org/A263827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * Σ_{d|n, n/d odd} σ(d) = 2 * σ(2^k) * Σ_{b|m} σ(b),
 *            where n = 2^k * m, m odd
 * Range:     u8 n <= 41, u16 n <= 5039, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 86 terms through n = 8940 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* cur_sigma is the product of σ(pᵉ) for the primes processed so far. */
static void div_sigma_rec(int level, seq_t cur_sigma,
                          const uint32_t *pf,
                          const size_t *cexp,
                          int cnt,
                          seq_t *sum)
{
    if (level == cnt) {
        *sum += cur_sigma;
        return;
    }
    uint32_t p = pf[level];
    size_t e = cexp[level];
    seq_t sigma_factor = 1;   /* σ(p⁰) = 1 */
    seq_t power = 1;          /* p⁰ */
    /* exponent 0 */
    div_sigma_rec(level + 1, cur_sigma, pf, cexp, cnt, sum);
    /* exponents 1 .. e */
    for (size_t exp = 1; exp <= e; ++exp) {
        power *= (seq_t)p;
        sigma_factor += power;  /* accumulate σ(p^exp) = 1 + p + … + p^exp */
        div_sigma_rec(level + 1, cur_sigma * sigma_factor, pf, cexp,
                      cnt, sum);
    }
}

#define PRIME_LIMIT 100000
static bool is_composite[PRIME_LIMIT + 1];
static uint32_t primes[PRIME_LIMIT];
static size_t primes_cnt = 0;
static bool primes_init = false;

static void init_primes(void)
{
    if (primes_init)
        return;
    primes_cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            primes[primes_cnt++] = (uint32_t)i;
            if (i * i <= PRIME_LIMIT) {
                for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    primes_init = true;
}

/* Recursive enumeration of divisors of an odd number. */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* out of defined range, harmless */

    /* Separate factors of 2 */
    idx_t m = n;
    size_t k = 0;
    while ((m & ((idx_t)1)) == 0) {
        ++k;
        m >>= 1;
    }

    /* σ(2^k) = Σ_{i=0}^k 2^i */
    seq_t sigma_2k = 1;   /* 2^0 */
    seq_t pow2 = 1;
    for (size_t i = 0; i < k; ++i) {
        pow2 *= (seq_t)2;
        sigma_2k += pow2;
    }

    /* Factor odd part m */
    init_primes();
    uint32_t pfact[64];
    size_t efact[64];
    int cnt = 0;
    idx_t tmp = m;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            size_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            pfact[cnt] = p;
            efact[cnt] = e;
            ++cnt;
        }
    }
    if (tmp > 1) {
        pfact[cnt] = (uint32_t)tmp;
        efact[cnt] = 1;
        ++cnt;
    }

    /* Sum σ(b) over all divisors b of m */
    seq_t sumDivSigs = 0;
    div_sigma_rec(0, (seq_t)1, pfact, efact, cnt, &sumDivSigs);

    /* Final result: 2 * σ(2^k) * Σ_{b|m} σ(b) */
    seq_t result = sigma_2k * sumDivSigs * 2;
    return result;
}
