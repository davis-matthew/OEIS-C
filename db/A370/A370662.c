/*
 * A370662  Numbers m such that 3 does not divide the m-th Catalan number
 *          A000108(m); m such that A067397(m) = 0.
 * https://oeis.org/A370662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*A005836((n+3)/3) + offset where offset = -1 if n%3==0,
 *            0 if n%3==1, +1 if n%3==2. A005836(k) is the k‑th number whose
 *            ternary representation contains only digits 0 and 1; it is
 *            obtained by interpreting k-1 in binary as a base‑3 number
 * Range:     u8 n <= 58, u16 n <= 1919, u32 n <= 9533, u64 n <= 9533,
 *            u128 n <= 9533, big any n
 * Helpers:   a005836_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a005836_value.h"

seq_t a(idx_t n)
{
    // Sequence is defined for n >= 1; for n == 0 we can return 0.
    if (n == 0)
        return 0;

    idx_t k = (n + 3) / 3;             /* determine k for this n */
    seq_t val = a005836_value(k);      /* A005836(k) */

    switch (n % 3) {
    default:   /* 0 */ return val * 3 - 1;
    case 1:    return val * 3;
    case 2:    return val * 3 + 1;
    }
}
