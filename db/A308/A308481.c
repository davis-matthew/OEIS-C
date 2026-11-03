/*
 * A308481  a(n) = Sum_{k=1..n, gcd(n,k) = 1} k^n.
 * https://oeis.org/A308481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n, gcd(n,k)=1} k^n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 346 (2026-11-01)
 * Helpers:   gcd, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        if (gcd((seq_t)k, (seq_t)n) == 1) {
            sum += ipow((seq_t)k, (seq_t)n);
        }
    }
    return sum;
}
