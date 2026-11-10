/*
 * A287375  Positions of 2 in A101666.
 * https://oeis.org/A287375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th (1-based) position k+1 of a 2 in A101666 (A101666
 *            fixed point of 0->01, 1->12, 2->10 from offset 0); found by
 *            scanning odd k, since a(n) = 2*A287374(n) is even
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 8558 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A101666_sym(idx_t n)
{
    int val = 0;
    if (n == 0) return 0;
    int top = 63;
    while (!((n >> top) & 1)) top--;
    for (int i = top; i >= 0; i--) {
        int bit = (int)((n >> i) & 1);
        if (val == 0)      val = bit ? 1 : 0;
        else if (val == 1) val = bit ? 2 : 1;
        else               val = bit ? 0 : 1;
    }
    return val;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 1;
    for (;; k += 2) {
        if (A101666_sym(k) == 2) {
            count++;
            if (count == n) break;
        }
    }
    return (seq_t)k + 1;
}
