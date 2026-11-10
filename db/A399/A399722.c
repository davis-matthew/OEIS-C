/*
 * A399722  a(n) = 3^(m-1) + Sum_{i=2..m} 3^(m-i)*2^(d(1)+...d(i-1)) where
 *          m=A122458(n) and d(i)=A351122(n,i-1) for 1<=i<m.
 * https://oeis.org/A399722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A060565(n)*2^A260590(n) - (2*n+1)*3^A122458(n); here x =
 *            A060565(n), k = A260590(n), m = A122458(n)
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t start = (seq_t)2 * (seq_t)n + 1;
    seq_t x = start;
    unsigned long long k = 0, m = 0;

    while (x >= start) {
        if ((x & 1) == 0) {
            x >>= 1;
        } else {
            x = (x * 3 + 1) >> 1;
            m++;
        }
        k++;
    }

    seq_t p2 = x;
    for (unsigned long long i = 0; i < k; i++) p2 = p2 * 2;
    seq_t p3 = start;
    for (unsigned long long i = 0; i < m; i++) p3 = p3 * 3;

    return p2 - p3;
}
