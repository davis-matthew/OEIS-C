/*
 * A343652  Number of maximal pairwise coprime sets of divisors of n.
 * https://oeis.org/A343652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Bell(#distinct primes of n) * ∏(exponent of each prime
 *            factor)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of maximal pairwise coprime sets of divisors of n */
#include <stdint.h>
static const seq_t _bell[6] = {1,1,2,5,15,52};

seq_t a(idx_t n) {
    /* n >= 1 */
    idx_t temp = n;
    idx_t k = 0;
    seq_t prod = 1;

    /* Trial division */
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            ++k;
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            prod *= (seq_t)e;
        }
    }
    if (temp > 1) {
        /* Remaining prime factor with exponent 1 */
        ++k;
        /* multiply by 1 -> no change */
    }

    /* k <= 5 for n <= 10000, otherwise the sequence is not defined by this implementation */
    return prod * _bell[k];
}
