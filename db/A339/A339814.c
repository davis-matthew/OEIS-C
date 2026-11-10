/*
 * A339814  The exponent of the highest power of 2 dividing (A019565(2n) - 1).
 * https://oeis.org/A339814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007814(A019565(2n) - 1), where A019565(2n) = Product_{k
 *            in set bits of n} prime(k+2)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9779 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint32_t A339814_prime_after(uint32_t idx)
{
    /* returns prime(idx+2), the (idx+2)-th prime, by trial division */
    uint32_t count = 0, c = 1;
    while (count < idx + 2) {
        c++;
        bool ok = true;
        for (uint32_t d = 2; d * d <= c; d++) {
            if (c % d == 0) { ok = false; break; }
        }
        if (ok) count++;
    }
    return c;
}

seq_t a(idx_t n)
{
    seq_t prod = 1;
    uint32_t bit = 0;
    idx_t m = n;
    while (m != 0) {
        if (m & 1) prod *= (seq_t)A339814_prime_after(bit);
        m >>= 1;
        bit++;
    }
    seq_t x = prod - 1;
    if (x == 0) return 0;
    seq_t cnt = 0;
    while ((x & 1) == 0) {
        cnt++;
        x >>= 1;
    }
    return cnt;
}
