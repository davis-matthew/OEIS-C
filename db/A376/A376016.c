/*
 * A376016  a(n) = Sum_{d|n} d^(n/d) * binomial(n/d,d).
 * https://oeis.org/A376016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(n/d) * binomial(n/d,d)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 44, u64 n <= 98, u128 n <= 212,
 *            big any n
 * Helpers:   big_pow, binomial, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* Undefined for n < 1, return 0 as safety. */

    /* ---------- factorise n ---------- */
    const size_t MAX_FACTORS = 64;
    uint64_t primes[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t prime_count = 0;
    factor_u64(n, primes, exps, &prime_count);

    /* ---------- generate all positive divisors of n ---------- */
    size_t div_cap = 16;
    size_t div_cnt = 1;
    uint64_t *divs = (uint64_t *)malloc(sizeof(uint64_t) * div_cap);
    if (!divs)
        return 0;
    divs[0] = 1;

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p   = primes[i];
        uint64_t exp = exps[i];
        size_t old_cnt = div_cnt;
        uint64_t pk = 1;
        for (uint64_t k = 1; k <= exp; ++k) {
            pk *= p; /* p^k */
            for (size_t j = 0; j < old_cnt; ++j) {
                if (div_cnt == div_cap) {
                    div_cap <<= 1;
                    uint64_t *tmp = (uint64_t *)realloc(divs, sizeof(uint64_t) * div_cap);
                    if (!tmp) {
                        free(divs);
                        return 0;
                    }
                    divs = tmp;
                }
                divs[div_cnt++] = divs[j] * pk;
            }
        }
    }

    /* ---------- compute the sum over all divisors ---------- */
    seq_t total = 0;

    for (size_t i = 0; i < div_cnt; ++i) {
        uint64_t d  = divs[i];
        uint64_t nd = n / d; /* n/d */

        if (nd < d)
            continue; /* binomial(nd,d) == 0 for nd < d */

        seq_t pow_term = big_pow((seq_t)d, (seq_t)nd);
        seq_t bin_term = binomial((idx_t)nd, (idx_t)d);
        seq_t term     = pow_term * bin_term;

        total += term;
    }

    free(divs);
    return total;
}
