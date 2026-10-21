/*
 * A254065  Vulgar fractions whose denominators are numbers ending with nine,
 *          the case 1/19.
 * https://oeis.org/A254065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (2^(n-1) mod 19) mod 10
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t pow2_mod19[18] = {
    1ULL,  2ULL,  4ULL,  8ULL, 16ULL, 13ULL, 7ULL, 14ULL,
    9ULL, 18ULL, 17ULL, 15ULL, 11ULL, 3ULL, 6ULL, 12ULL,
    5ULL, 10ULL
};

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence */
    idx_t exp = (n - 1) % 18ULL;
    seq_t val = pow2_mod19[exp];
    return val % 10ULL;
}
