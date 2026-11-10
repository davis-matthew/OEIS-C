/*
 * A372287  Array read by upward antidiagonals: A(n, k) = A371092(A372283(n,
 *          k)), n,k >= 1.
 * https://oeis.org/A372287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A371092(A372283(n)), where A372283 is the array A(r,c)
 *            read by upward antidiagonals with A(1,c)=2c-1 and
 *            A(r,c)=odd_part(3*A(r-1,c)+1), and
 *            A371092(m)=floor((odd_part(3m+1)+5)/6)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-10)
 * Helpers:   odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/odd_part.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t d = 1;
    while (d * (d + 1) / 2 < n) ++d;
    idx_t p = n - d * (d - 1) / 2;
    idx_t col = p;
    idx_t row = d - p + 1;

    seq_t v = (seq_t)2 * (seq_t)col - (seq_t)1;
    for (idx_t r = 2; r <= row; ++r)
        v = odd_part((seq_t)3 * v + (seq_t)1);

    seq_t x = odd_part((seq_t)3 * v + (seq_t)1);
    return (x + 5) / 6;
}
