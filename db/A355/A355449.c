/*
 * A355449  a(n) = 1 if n^2 + 2 is prime, 0 otherwise.
 * https://oeis.org/A355449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n^2 + 2 is prime, 0 otherwise
 * Range:     u8 n <= 15, u16 n <= 9736, u32 n <= 9736, u64 n <= 9736,
 *            u128 n <= 9736, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    seq_t val = ((seq_t)n) * ((seq_t)n) + 2;
    return is_prime_big(val) ? 1 : 0;
}
