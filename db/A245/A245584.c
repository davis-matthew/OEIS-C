/*
 * A245584  Let f(m) put the leftmost digit of the positive integer m at its
 *          end; a(n) is the sequence of all positive integers m with
 *          f^2(m)=f(m^2).
 * https://oeis.org/A245584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(1)=1, a(2)=2, a(3)=3, a(n)=1 followed by (n-3) digits '2' for
 *            n>=4
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;
    idx_t k = n - 3;      // number of '2's to append
    seq_t v = (seq_t)1;   // starting with the leading '1'
    for (idx_t i = 0; i < k; ++i) {
        v = v * 10 + 2;
    }
    return v;
}
