/*
 * A378537  Characteristic function for primitive Zumkeller numbers.
 * https://oeis.org/A378537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the characteristic function of primitive Zumkeller
 *            numbers (A378537). a(n) = 1 iff n is a Zumkeller number and no
 *            proper divisor of n is Zumkeller. The function below computes
 *            this value for every n >= 1 using an explicit subset‑sum test on
 *            the divisors of n
 * Range:     u8 n <= 1633, u16 n <= 9278, u32 n <= 9278, u64 n <= 9278,
 *            u128 n <= 9278, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"

/* Generates all divisors of n from its prime factorization.
   primes[k] and exps[k] give the k‑th prime factor and its exponent.
   The array `divs` must be large enough to hold all divisors.  */
static void gen_divisors_rec(size_t idx, uint64_t current,
                             const uint64_t *primes, const uint64_t *exps,
                             size_t n_factors,
                             uint64_t *divs, size_t *cnt)
{
    if (idx == n_factors) {
        divs[(*cnt)++] = current;
        return;
    }
    uint64_t p = primes[idx];
    uint64_t power = 1;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        gen_divisors_rec(idx + 1, current * power, primes, exps,
                         n_factors, divs, cnt);
        power *= p;
    }
}

/* Helper to generate all divisors of n.  Returns the number of divisors
   and stores them in `divs`. */
static size_t generate_divisors(uint64_t n, uint64_t *divs)
{
    uint64_t pr[64], ex[64];
    size_t cnt_factors = 0;
    factor_u64(n, pr, ex, &cnt_factors);

    size_t div_cnt = 0;
    gen_divisors_rec(0, 1ULL, pr, ex, cnt_factors, divs, &div_cnt);
    return div_cnt;
}

/* Computes the sum of divisors sigma(n).  Uses the factorization. */
static seq_t sigma_from_factors(const uint64_t *pr, const uint64_t *ex,
                                size_t cnt_factors)
{
    seq_t sigma = 1;
    for (size_t i = 0; i < cnt_factors; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        seq_t term = 1;
        for (uint64_t j = 0; j < e; ++j) term *= p;
        term = (term * p - 1) / (p - 1);
        sigma *= term;
    }
    return sigma;
}

/* Determines whether n is a Zumkeller number.  Returns true if
   the divisors of n can be partitioned into two subsets with equal sum. */
static bool is_zumkeller(uint64_t n)
{
    if (n < 2) return false;     /* 1 is not Zumkeller */

    uint64_t pr[64], ex[64];
    size_t cnt_factors = 0;
    factor_u64(n, pr, ex, &cnt_factors);

    seq_t sigma = sigma_from_factors(pr, ex, cnt_factors);
    if (((seq_t)1 & sigma) != 0) return false;  /* sigma odd */

    uint64_t target = (uint64_t)(sigma / 2);

    /* Generate all divisors */
    uint64_t divs[512];
    size_t div_cnt = generate_divisors(n, divs);

    /* Subset sum DP using a byte array */
    uint8_t *dp = (uint8_t *)calloc(target + 1, sizeof(uint8_t));
    if (!dp) return false; /* OOM fallback */
    dp[0] = 1;
    for (size_t i = 0; i < div_cnt; ++i) {
        uint64_t d = divs[i];
        if (d > target) continue;
        for (uint64_t s = target; s >= d; --s) {
            if (dp[s - d]) dp[s] = 1;
        }
    }
    bool result = dp[target] != 0;
    free(dp);
    return result;
}

/* Determines whether n is a primitive Zumkeller number.
   It is primitive if it is Zumkeller and no proper divisor (>=2)
   is Zumkeller. */
static bool is_primitive_zumkeller(uint64_t n)
{
    if (!is_zumkeller(n)) return false;

    /* Generate all divisors again */
    uint64_t divs[512];
    size_t div_cnt = generate_divisors(n, divs);
    for (size_t i = 0; i < div_cnt; ++i) {
        uint64_t d = divs[i];
        if (d > 1 && d < n && is_zumkeller(d))
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    return (seq_t)(is_primitive_zumkeller((uint64_t)n));
}
