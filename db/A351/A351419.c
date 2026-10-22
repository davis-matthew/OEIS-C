/*
 * A351419  If n = p_1^e_1 * ... * p_k^e_k, where p_1 < ... < p_k are primes,
 *          then a(n) is obtained by replacing the last factor p_k^e_k by (p_k
 *          - 1)^(e_k + 1); a(1) = 1.
 * https://oeis.org/A351419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p₁^e₁ · … · p_{k-1}^e_{k-1} · (p_k-1)^(e_k+1), where n =
 *            p₁^e₁ · … · p_k^e_k (p₁ < … < p_k). a(1) = 1
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static inline seq_t pow_seq(seq_t base, uint64_t exp)
{
    seq_t r = 1;
    while (exp > 0) {
        r *= base;
        --exp;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    uint64_t tmp = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t factor_cnt = 0;

    /* Handle factor 2. */
    if ((tmp & 1ULL) == 0) {
        uint64_t e = 0;
        while ((tmp & 1ULL) == 0) {
            tmp >>= 1;
            ++e;
        }
        primes[factor_cnt] = 2;
        exps[factor_cnt] = e;
        ++factor_cnt;
    }

    /* Trial division for odd primes up to sqrt(tmp). */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[factor_cnt] = p;
            exps[factor_cnt] = e;
            ++factor_cnt;
        }
    }

    /* Remaining prime factor, if any. */
    if (tmp > 1) {
        primes[factor_cnt] = tmp;
        exps[factor_cnt] = 1;
        ++factor_cnt;
    }

    seq_t result = 1;

    /* Product of all prime powers except the last one. */
    if (factor_cnt > 1) {
        for (size_t i = 0; i < factor_cnt - 1; ++i) {
            result *= pow_seq((seq_t)primes[i], exps[i]);
        }
    }

    /* Multiply by the modified last factor. */
    seq_t base_last = (seq_t)(primes[factor_cnt - 1] - 1);
    uint64_t exp_last = exps[factor_cnt - 1] + 1;
    result *= pow_seq(base_last, exp_last);

    return result;
}
