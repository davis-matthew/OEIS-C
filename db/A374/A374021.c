/*
 * A374021  Row products of A351153.
 * https://oeis.org/A374021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} T(n,k), where T(n,k) = 1 + (k-1)*(n+1) -
 *            (k-1)*k/2 = n*(k-1) - k*(k-3)/2; a(0) = 1 (empty product)
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
    seq_t sn = (seq_t)n;
    seq_t p = 1;
    for (idx_t k = 1; k <= n; k++) {
        seq_t sk = (seq_t)k;
        seq_t t = 1 + (sk - 1) * (sn + 1) - (sk - 1) * sk / 2;
        p *= t;
    }
    return p;
}
