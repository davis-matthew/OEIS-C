/*
 * A350548  Irregular triangle T(n,k) read by rows in which row n lists the
 *          iterates of the A350515 map from n to 0.
 * https://oeis.org/A350548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   T(n,0) = n; T(n,k) = A350515(T(n,k-1)); row n ends at 0. a(m) is
 *            the m-th entry of the flattened triangle (found by walking rows)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Validated: partial, pass, 82 terms through n = 81 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A350548_step(seq_t x)
{
    seq_t r3 = x % (seq_t)3;
    if (r3 == (seq_t)1)
        return (x - (seq_t)1) / (seq_t)3;
    seq_t r6 = x % (seq_t)6;
    if (r6 == (seq_t)0 || r6 == (seq_t)2)
        return x / (seq_t)2;
    return ((seq_t)3 * x + (seq_t)1) / (seq_t)2;
}

seq_t a(idx_t n)
{
    idx_t pos = n;
    idx_t row = 0;
    for (;;) {
        seq_t x = (seq_t)row;
        idx_t len = 1;
        while (x != (seq_t)0) {
            x = A350548_step(x);
            len++;
        }
        if (pos < len) {
            x = (seq_t)row;
            for (idx_t i = 0; i < pos; i++)
                x = A350548_step(x);
            return x;
        }
        pos -= len;
        row++;
    }
}
