/*
 * A351758  a(n) = Sum_{p|n, p prime} Sum_{d|n} lcm(d,p).
 * https://oeis.org/A351758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} Sum_{d|n} lcm(d,p)
 * Range:     u8 n <= 29, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: generate all divisors of n (prime factorised by p[], e[]) */
static void gen_divisors(const uint64_t p[], const uint8_t e[],
                         int fac_cnt,
                         uint64_t cur, int idx,
                         uint64_t *divs, size_t *cnt)
{
    if (idx == fac_cnt) {
        divs[(*cnt)++] = cur;
        return;
    }
    uint64_t val = cur;
    for (int k = 0; k <= (int)e[idx]; ++k) {
        gen_divisors(p, e, fac_cnt, val, idx + 1, divs, cnt);
        val *= p[idx];
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2) {
        /* n = 1 returns 0 because there are no prime divisors */
        return (seq_t)0;
    }

    /* Factorise n */
    uint64_t temp = (uint64_t)n;
    uint64_t primes[10];
    uint8_t  exps  [10];
    int      fac_cnt = 0;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            primes[fac_cnt] = p;
            uint8_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            exps[fac_cnt] = e;
            ++fac_cnt;
        }
    }
    if (temp > 1) {
        primes[fac_cnt] = temp;
        exps[fac_cnt]  = 1;
        ++fac_cnt;
    }

    /* Sum of distinct prime divisors */
    seq_t sumP = 0;
    for (int i = 0; i < fac_cnt; ++i)
        sumP += (seq_t)primes[i];

    /* Generate divisors (max 128 for n <= 10000) */
    uint64_t divs[128];
    size_t   div_cnt = 0;
    gen_divisors(primes, exps, fac_cnt, 1, 0, divs, &div_cnt);

    /* Sum of all divisors D */
    seq_t D = 0;
    for (size_t i = 0; i < div_cnt; ++i)
        D += (seq_t)divs[i];

    /* a(n) = D*sumP - Σ_{p|n} (p-1)*Dp, where Dp = Σ_{d|n, p|d} d */
    seq_t result = D * sumP;
    for (int i = 0; i < fac_cnt; ++i) {
        uint64_t p = primes[i];
        seq_t Dp = 0;
        for (size_t j = 0; j < div_cnt; ++j)
            if (divs[j] % p == 0)
                Dp += (seq_t)divs[j];
        result -= (seq_t)(p - 1) * Dp;
    }

    return result;
}
