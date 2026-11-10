/*
 * A374655  a(n) = (1/3)*A374654(n).
 * https://oeis.org/A374655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} (L(k)+1), L = Lucas numbers (A000032);
 *            equals A374654(n)/3, since L(0)+1 = 3. a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t prev = 2;   /* L(0) */
    seq_t cur  = 1;   /* L(1) */
    seq_t prod = 1;
    for (idx_t k = 1; k <= n; ++k) {
        prod = prod * (cur + 1);
        seq_t next = prev + cur;
        prev = cur;
        cur = next;
    }
    return prod;
}
