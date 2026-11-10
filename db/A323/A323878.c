/*
 * A323878  Number of divisors d of n such that A276086(d) divides n.
 * https://oeis.org/A323878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{d|n} [A276086(d)|n], where A276086(d) = Prod_k
 *            prime(k+1)^(digit k of d in primorial base)
 * Range:     u8 n <= 30030, u16 n <= 30030, u32 n <= 30030, u64 n <= 30030,
 *            u128 n <= 30030, big any n
 * Validated: standard, pass, 121 terms through n = 8439 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* returns true if A276086(d) divides n (stops early once the product exceeds n) */
static bool A323878_divides(uint64_t d, uint64_t n)
{
    static const uint64_t pr[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
    uint64_t r = 1;
    for (int k = 0; k < 16 && d > 0; ++k) {
        uint64_t base = pr[k];          /* primorial base: radix is prime(k+1) */
        uint64_t dig = d % base;
        d /= base;
        for (uint64_t j = 0; j < dig; ++j) {
            if (r > n / pr[k]) return false;
            r *= pr[k];
        }
    }
    return (n % r) == 0;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t cnt = 0;
    for (uint64_t i = 1; i <= N / i; ++i) {
        if (N % i != 0) continue;
        if (A323878_divides(i, N)) cnt++;
        uint64_t j = N / i;
        if (j != i && A323878_divides(j, N)) cnt++;
    }
    return (seq_t)cnt;
}
