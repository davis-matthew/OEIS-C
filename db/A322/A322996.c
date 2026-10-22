/*
 * A322996  Number of iterations of A049820(x) = x - A000005(x) needed to
 *          reach an odd number or zero, when starting from x = n.
 * https://oeis.org/A322996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n = 0 or n odd; * a(n) = 1 + a(n - d(n)) if n even,
 *            * where d(n) = A000005(n) = number of divisors of n
 * Range:     u8 n <= 2467, u16 n <= 9735, u32 n <= 9735, u64 n <= 9735,
 *            u128 n <= 9735, big any n
 * Helpers:   _num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_num_divisors.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n & 1)   return (seq_t)0;          /* odd n */

    seq_t steps = (seq_t)0;
    idx_t cur   = n;
    while (cur > 0 && (cur & 1) == 0) {    /* while even */
        steps += (seq_t)1;
        /* subtract number of divisors, cast to idx_t for subtraction */
        cur -= (idx_t)_num_divisors(cur);
    }
    return steps;
}
