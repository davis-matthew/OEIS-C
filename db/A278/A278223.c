/*
 * A278223  Least number with the same prime signature as the n-th odd number:
 *          a(n) = A046523(2n-1).
 * https://oeis.org/A278223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Least number with the same prime signature as 2*n-1
 *            (A046523(2n-1))
 * Range:     u8 n <= 1732, u16 n <= 8118, u32 n <= 8118, u64 n <= 8118,
 *            u128 n <= 8118, big any n
 * Helpers:   factor_u64, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_int.h"

/* first 16 primes, enough for all exponents that occur for 2*n-1 up to n≈10000 */
static const uint64_t PRIMES[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};

seq_t a(idx_t n)
{
    /* m = 2*n - 1, odd for all n >= 1 */
    uint64_t m = ((uint64_t)n << 1) - 1;

    /* factor m into primes and exponents */
    uint64_t primes_arr[64];
    uint64_t exps_arr[64];
    size_t cnt = 0;
    factor_u64(m, primes_arr, exps_arr, &cnt);

    /* if m == 1, its signature is empty → the least number is 1 */
    if (cnt == 0)
        return 1;

    /* sort exponents in decreasing order */
    for (size_t i = 1; i < cnt; ++i) {
        uint64_t key = exps_arr[i];
        int j = (int)i - 1;
        while (j >= 0 && exps_arr[(size_t)j] < key) {
            exps_arr[(size_t)(j + 1)] = exps_arr[(size_t)j];
            --j;
        }
        exps_arr[(size_t)(j + 1)] = key;
    }

    /* build the least number with the same exponent multiset */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = pow_int((seq_t)PRIMES[i], (idx_t)exps_arr[i]);
        result *= term;
    }

    return result;
}
