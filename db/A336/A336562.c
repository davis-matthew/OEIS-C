/*
 * A336562  Number of pairs sigma(p^x), sigma(q^y) that are not coprime, where
 *          p^x and q^y are any two maximal prime power divisors of n, with p
 *          < q. a(1) = 0.
 * https://oeis.org/A336562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered pairs of maximal prime‑power divisors
 *            p^x, q^y of n for which gcd(sigma(p^x), sigma(q^y)) > 1
 * Range:     u8 n <= 33, u16 n <= 1813, u32 n <= 9755, u64 n <= 9755,
 *            u128 n <= 9755, big any n
 * Helpers:   big_pow, factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"
#include "../utils/gcd.h"

/* Compute σ(p^e) = (p^(e+1) – 1) / (p – 1) for a prime power p^e   */
static seq_t sigma_prime_power(seq_t p, idx_t e)
{
    seq_t num = big_pow(p, (seq_t)(e + 1));
    seq_t den = p - 1;
    return (num - 1) / den;
}

seq_t a(idx_t n)
{
    /* n = 1 has no prime power divisors → answer 0 */
    if (n <= 1)
        return 0;

    /* Allocate space for factor arrays (max 64 distinct primes) */
    uint64_t *pr = (uint64_t*)malloc(64 * sizeof(uint64_t));
    uint64_t *ex = (uint64_t*)malloc(64 * sizeof(uint64_t));
    size_t cnt = 0;

    /* Factor n; pr[i] = prime, ex[i] = exponent */
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* Array of σ(p^e) values */
    seq_t *sigs = (seq_t*)malloc(cnt * sizeof(seq_t));
    for (size_t i = 0; i < cnt; ++i) {
        seq_t prime = (seq_t)pr[i];
        idx_t exp   = (idx_t)ex[i];
        sigs[i] = sigma_prime_power(prime, exp);
    }

    /* Count pairs with non‑unit gcd */
    uint64_t pair_cnt = 0;
    for (size_t i = 0; i < cnt; ++i) {
        for (size_t j = i + 1; j < cnt; ++j) {
            if (gcd(sigs[i], sigs[j]) != 1)
                ++pair_cnt;
        }
    }

    /* Free temporary storage */
    free(pr);
    free(ex);
    free(sigs);

    /* Convert count to seq_t before returning */
    return (seq_t)pair_cnt;
}
