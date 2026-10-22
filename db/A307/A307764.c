/*
 * A307764  Numbers m whose distinct prime factors are exactly the same as the
 *          distinct prime factors of each of the numbers obtained by deleting
 *          any single digit in the decimal expansion of m.
 * https://oeis.org/A307764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 25*10^(ceil(n/3)+1) if n mod 3 = 1, 36*10^(ceil(n/3)+1)
 *            if n mod 3 = 2, 96*10^(ceil(n/3)+1) if n mod 3 = 0
 * Range:     u8 none, u16 n <= 5, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* block = ceil(n/3) = (n + 2) / 3 */
    idx_t block = (n + 2) / 3;

    /* exponent for 10: e = block + 1 */
    idx_t e = block + 1;

    seq_t base = pow10(e);   /* 10^e */

    seq_t factor;
    switch (n % 3) {
        case 1: factor = 25; break;
        case 2: factor = 36; break;
        default: factor = 96; break;
    }

    return base * factor;
}
