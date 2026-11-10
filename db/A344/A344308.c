/*
 * A344308  Numbers k such that A205791(k) = k+1.
 * https://oeis.org/A344308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that k is squarefree and no prime p | k has p
 *            == 1 (mod 5) (i.e. x -> x^5 is a bijection mod k); found by
 *            search
 * Range:     u8 n <= 117, u16 n <= 1258, u32 n <= 1258, u64 n <= 1258,
 *            u128 n <= 1258, big any n
 * Validated: standard, pass, 86 terms through n = 1219 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A344308_good(uint64_t k)
{
    uint64_t m = k;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            m /= p;
            if (m % p == 0) return false;
            if (p % 5 == 1) return false;
        }
    }
    if (m > 1 && m % 5 == 1) return false;
    return true;
}

seq_t a(idx_t n)
{
    uint64_t cnt = 0;
    uint64_t k = 0;
    while (cnt < (uint64_t)n) {
        k++;
        if (A344308_good(k)) cnt++;
    }
    return (seq_t)k;
}
