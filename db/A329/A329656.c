/*
 * A329656  Starting at k = A001414(n), iterate k -> n mod k until k=0. a(n)
 *          is the number of steps taken.
 * https://oeis.org/A329656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of k -> n mod k starting at k = sum
 *            of prime factors of n (counting multiplicity) until k = 0. This
 *            implements OEIS sequence A329656
 * Range:     u8 n <= 2688, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sum_prime_factors_mult
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sum_prime_factors_mult.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    seq_t steps = 0;
    seq_t k = sum_prime_factors_mult(n);
    while (k != 0) {
        idx_t rem = n % (idx_t)k;
        steps++;
        k = (seq_t)rem;
    }
    return steps;
}
