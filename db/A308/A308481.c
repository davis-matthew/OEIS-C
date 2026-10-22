/*
 * A308481  a(n) = Sum_{k=1..n, gcd(n,k) = 1} k^n.
 * https://oeis.org/A308481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n, gcd(n,k)=1} k^n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        if (gcd((seq_t)k, (seq_t)n) == 1) {
            sum += big_pow((seq_t)k, (seq_t)n);
        }
    }
    return sum;
}
