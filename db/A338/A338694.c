/*
 * A338694  a(n) = Sum_{d|n} d^d * binomial(d, n/d).
 * https://oeis.org/A338694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^d * binomial(d, n/d)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

static seq_t seq_pow_uint64(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp) {
        if (exp & 1)
            result *= b;
        exp >>= 1;
        if (exp)
            b *= b;
    }
    return result;
}

static seq_t binomial_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    idx_t m = (k <= n - k) ? k : n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= m; ++i)
        res = res * ((seq_t)n - i + 1) / (seq_t)i;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* out‑of‑range input */

    /* factor n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    uint64_t temp = n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (temp > 1) {
        primes[cnt] = temp;
        exps[cnt] = 1;
        ++cnt;
    }

    /* generate all divisors of n */
    enum { DIV_CAP = 8192 };
    uint64_t divs[DIV_CAP];
    size_t div_count = 1;
    divs[0] = 1ULL;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t mult = 1;
        size_t base_count = div_count;
        for (uint64_t e = 1; e <= exps[i]; ++e) {
            mult *= p;
            for (size_t j = 0; j < base_count; ++j) {
                if (div_count >= DIV_CAP)
                    continue;   /* safety, should never happen for n <= 10⁴ */
                divs[div_count++] = divs[j] * mult;
            }
        }
    }

    /* compute the sum */
    seq_t sum = 0;
    for (size_t idx = 0; idx < div_count; ++idx) {
        uint64_t d = divs[idx];
        uint64_t t = n / d;          /* n/d */
        if (t > d)
            continue;                 /* binomial will be zero */
        seq_t bin = binomial_small((idx_t)d, (idx_t)t);
        if (bin == 0) continue;
        seq_t pow_d_d = seq_pow_uint64((seq_t)d, (idx_t)d);
        sum += pow_d_d * bin;
    }

    return sum;
}
