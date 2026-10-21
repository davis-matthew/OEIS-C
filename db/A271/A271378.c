/*
 * A271378  a(n) = 5^n mod 31.
 * https://oeis.org/A271378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) with a(0)=1, a(1)=5, a(2)=25
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t tbl[3] = { (seq_t)1, (seq_t)5, (seq_t)25 };
    return tbl[(size_t)(n % 3)];
}
