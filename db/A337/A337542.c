/*
 * A337542  Number of proper divisors d of n for which sigma(A003961(d)) >=
 *          2*sigma(d), where sigma is the sum of divisors, and A003961(x)
 *          shifts the prime factorization of x one step towards larger
 *          primes.
 * https://oeis.org/A337542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of proper divisors d of n such that
 *            sigma(A003961(d)) >= 2 * sigma(d)
 * Range:     u8 n <= 33, u16 n <= 4595, u32 n <= 8603, u64 n <= 8603,
 *            u128 n <= 8603, big any n
 * Helpers:   factor_u64, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation enumerates proper divisors of n and checks
   the inequality by factoring each divisor. */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_small.h"

/* Return the next prime after p (p is prime). */
static uint64_t next_prime_of_prime(uint64_t p)
{
    uint64_t cand = p + 1;
    while (!is_prime_small((seq_t)cand))
        ++cand;
    return cand;
}

/* Compute sigma(x) from prime factorization arrays. */
static seq_t sigma_of_factors(const uint64_t *pr, const uint64_t *ex, size_t cnt)
{
    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        seq_t p_pow = 1;
        for (uint64_t j = 0; j < e + 1; ++j)
            p_pow *= (seq_t)p;
        seq_t comp = (p_pow - 1) / ((seq_t)p - 1);
        sigma *= comp;
    }
    return sigma;
}

/* Compute sigma(A003961(x)) from prime factorization arrays. */
static seq_t sigma_shift_of_factors(const uint64_t *pr, const uint64_t *ex, size_t cnt)
{
    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        uint64_t np = next_prime_of_prime(p);
        seq_t p_pow = 1;
        for (uint64_t j = 0; j < e + 1; ++j)
            p_pow *= (seq_t)np;
        seq_t comp = (p_pow - 1) / ((seq_t)np - 1);
        sigma *= comp;
    }
    return sigma;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t count = 0;

    /* Iterate proper divisors by scanning up to sqrt(n). */
    idx_t sq = 0;
    while ((sq + 1) * (sq + 1) <= n)
        ++sq;

    for (idx_t d = 1; d <= sq; ++d) {
        if (n % d != 0)
            continue;
        idx_t q = n / d;

        /* Test divisor d. */
        if (d < n) {
            uint64_t fpr[64], fex[64];
            size_t fcnt;
            factor_u64(d, fpr, fex, &fcnt);
            seq_t sigma_d   = sigma_of_factors(fpr, fex, fcnt);
            seq_t sigma_rev = sigma_shift_of_factors(fpr, fex, fcnt);
            if (sigma_rev >= sigma_d + sigma_d)
                ++count;
        }

        /* Test complementary divisor q. */
        if (q != d && q < n) {
            uint64_t fpr[64], fex[64];
            size_t fcnt;
            factor_u64(q, fpr, fex, &fcnt);
            seq_t sigma_d   = sigma_of_factors(fpr, fex, fcnt);
            seq_t sigma_rev = sigma_shift_of_factors(fpr, fex, fcnt);
            if (sigma_rev >= sigma_d + sigma_d)
                ++count;
        }
    }

    return count;
}
