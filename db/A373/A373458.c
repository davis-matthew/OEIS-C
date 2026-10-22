/*
 * A373458  Expansion of Sum_{p prime} x^p/(1 - p*x^p).
 * https://oeis.org/A373458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p|n, p prime} p^(n/p - 1) (OEIS A373458)
 * Range:     u8 n <= 17, u16 n <= 33, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   big_pow, factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 1 */
    if (n == 1)
        return 0;

    /* Factor n; at most 64 prime factors may appear for n <= UINT64_MAX */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t pf_cnt = 0;

    factor_u64((uint64_t)n, pr, ex, &pf_cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < pf_cnt; ++i) {
        idx_t p   = (idx_t)pr[i];
        idx_t exp = n / p - 1;
        seq_t term = big_pow((seq_t)p, (seq_t)exp);
        sum += term;
    }
    return sum;
}
