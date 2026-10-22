/*
 * A319279  Numbers that are congruent to {0, 3, 7, 10} mod 12.
 * https://oeis.org/A319279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*floor((n-1)/4) + [0,3,7,10]((n-1) mod 4)
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

extern seq_t a(idx_t n)
{
    /* offset values for the four congruence classes modulo 12 */
    static const seq_t offs[4] = { (seq_t)0, (seq_t)3, (seq_t)7, (seq_t)10 };
    /* n is guaranteed ≥ 1 by the problem statement */
    idx_t k = n - 1;               /* zero‑based index of the term */
    seq_t q = (seq_t)k / 4;        /* number of complete 12‑step cycles */
    seq_t r = (seq_t)k % 4;        /* position inside the current cycle */
    return 12 * q + offs[(int)r];
}
