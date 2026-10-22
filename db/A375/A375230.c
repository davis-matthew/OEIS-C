/*
 * A375230  The total number of infinitary divisors of the infinitary divisors
 *          of n.
 * https://oeis.org/A375230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult, changed
 * Formula:   a(n) = 3^sum_{p^e||n} popcount(e)
 * Range:     u8 n <= 7559, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n) {
    /* Factor n into prime powers */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Sum the binary digit sums (popcount) of the exponents */
    unsigned int sum_pop = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        while (e) {
            sum_pop++;
            e &= e - 1;   /* clear lowest set bit */
        }
    }

    /* Return 3^sum_pop as seq_t */
    return pow_int((seq_t)3, sum_pop);
}
