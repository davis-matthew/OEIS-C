/*
 * A374442  Row sums of A374443.
 * https://oeis.org/A374442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} rad(gcd(n,k)) = rad(n) + Sum_{d|n}
 *            rad(d)*phi(n/d) for n>0; a(0) = 1
 * Range:     u8 n <= 59, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 60 (2026-11-10)
 * Helpers:   radical, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/radical.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t s = radical(n);
    for (idx_t d = 1; d <= n; d++) {
        if (n % d == 0) {
            s += radical(d) * totient(n / d);
        }
    }
    return s;
}
