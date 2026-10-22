/*
 * A393053  Number of divisors d of n such that d and A276086(n) are coprime,
 *          where A276086 is the primorial base exp-function.
 * https://oeis.org/A393053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of divisors d of n such that gcd(d, A276086(n)) =
 *            1. A276086(n) = ∏ p_i^{d_i} where (d_i) are the digits of n in
 *            primorial base; a divisor is coprime to A276086(n) iff it
 *            contains no prime factor that appears with a non‑zero digit in
 *            that expansion
 * Range:     u8 n <= 9322, u16 n <= 9322, u32 n <= 9322, u64 n <= 9322,
 *            u128 n <= 9322, big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* utility headers */
#include "../utils/build_primes_upto.h"   /* build_primes_upto */
#include "../utils/factor_u64.h"          /* factor_u64 */

seq_t a(idx_t n)
{
    if (n == 0ULL)            /* undefined for the offset, but keep safe */
        return 0;

    /* ---- 1. Compute the primes that appear with a non‑zero digit
        in the primorial base expansion of n.  ---- */
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes_upto(n, &primes, &primes_cnt);   /* primes ≤ n */

    uint64_t digit_primes[32];
    size_t digit_cnt = 0;

    idx_t tmp = n;
    size_t pi = 0;                     /* index into primes array */
    while (tmp > 0 && pi < primes_cnt) {
        uint64_t prime  = primes[pi];
        uint64_t digit  = tmp % prime;
        if (digit > 0ULL)
            digit_primes[digit_cnt++] = prime;
        tmp /= prime;
        ++pi;
    }
    free(primes);

    /* ---- 2. Factor n. ---- */
    uint64_t fac_pr[64];
    uint64_t fac_ex[64];
    size_t fac_cnt;
    factor_u64((uint64_t)n, fac_pr, fac_ex, &fac_cnt);

    /* ---- 3. Count divisors that use only primes not in digit_primes. ---- */
    seq_t result = 1;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t p = fac_pr[i];

        /* check if p is among the digit primes */
        bool forbid = false;
        for (size_t j = 0; j < digit_cnt; ++j) {
            if (digit_primes[j] == p) {
                forbid = true;
                break;
            }
        }
        if (!forbid)
            result *= (seq_t)(fac_ex[i] + 1);
    }

    return result;
}
