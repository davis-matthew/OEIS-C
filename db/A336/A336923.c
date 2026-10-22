/*
 * A336923  a(n) = 1 if sigma(2n) - sigma(n) is a power of 2, otherwise 0.
 * https://oeis.org/A336923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = 1 if sigma(2n) - sigma(n) is a power of 2, else 0
 * Range:     u8 n <= 83, u16 n <= 7432, u32 n <= 7432, u64 n <= 7432,
 *            u128 n <= 7432, big any n
 * Helpers:   is_power_of_two, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/is_power_of_two.h"

seq_t a(idx_t n)
{
    seq_t sigma_n  = sigma_of(n);
    seq_t sigma_2n = sigma_of(n * 2);
    seq_t diff     = sigma_2n - sigma_n;
    return is_power_of_two(diff) ? (seq_t)1 : (seq_t)0;
}
