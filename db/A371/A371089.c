/*
 * A371089  Numbers k such that A003557(k) does not divide A276086(k), where
 *          A003557(k) is k divided by its largest squarefree divisor, and
 *          A276086 is the primorial base exp-function.
 * https://oeis.org/A371089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A003557(k) does not divide A276086(k);
 *            found by search, checking exponents of the primorial-base digits
 *            against the prime exponents of k
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9609 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A371089_member(uint64_t k)
{
    static const uint32_t pr[20] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71};
    uint64_t d[20];
    uint64_t m = k;
    for (int i = 0; i < 20; i++) {
        d[i] = m % pr[i];
        m /= pr[i];
    }
    uint64_t t = k;
    for (uint64_t p = 2; p * p <= t; p++) {
        if (t % p) continue;
        uint64_t e = 0;
        while (t % p == 0) { t /= p; e++; }
        if (e >= 2) {
            uint64_t dig = 0;
            for (int i = 0; i < 20; i++)
                if (pr[i] == p) { dig = d[i]; break; }
            if (dig < e - 1) return true;
        }
    }
    /* remaining t is 1 or a prime with exponent 1 */
    return false;
}

seq_t a(idx_t n)
{
    uint64_t cnt = 0;
    uint64_t k = 0;
    while (cnt < (uint64_t)n) {
        k++;
        if (A371089_member(k)) cnt++;
    }
    return (seq_t)k;
}
