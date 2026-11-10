/*
 * A376406  a(0) = 1, and for n > 0, a(n) = A019565(Sum_{i=0..n-1} a(i)),
 *          where A019565 is the base-2 exp-function.
 * https://oeis.org/A376406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = A019565(Sum_{i=0..n-1} a(i)) for n > 0, where
 *            A019565(m) = Product_{k in bits of m} prime(k+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 10 terms through n = 9 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A376406_isp(uint64_t n)
{
    if (n < 2) return false;
    if (n < 4) return true;
    if ((n & 1) == 0 || n % 3 == 0) return false;
    for (uint64_t d = 5; d * d <= n; d += 6)
        if (n % d == 0 || n % (d + 2) == 0) return false;
    return true;
}

static seq_t A376406_exp(seq_t m)
{
    seq_t result = 1;
    uint64_t p = 2;
    while (m != 0) {
        if ((m & 1) != 0) result *= (seq_t)p;
        m >>= 1;
        if (m == 0) break;
        do { ++p; } while (!A376406_isp(p));
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t cur = 1;
    seq_t sum = 1;
    for (idx_t i = 1; i <= n; ++i) {
        cur = A376406_exp(sum);
        sum += cur;
    }
    return cur;
}
