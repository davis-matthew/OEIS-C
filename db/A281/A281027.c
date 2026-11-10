/*
 * A281027  Partial products of A066570.
 * https://oeis.org/A281027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n} A066570(i), where A066570(i) =
 *            Product_{k<=i, gcd(k,i)>1} k (empty product = 1)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 16 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A281027_gcd(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        for (idx_t k = 2; k <= i; ++k) {
            if (A281027_gcd(k, i) > 1)
                res *= (seq_t)k;
        }
    }
    return res;
}
