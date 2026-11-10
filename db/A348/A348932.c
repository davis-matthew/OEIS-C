/*
 * A348932  Numbers k congruent to 1 or 5 mod 6, for which A348930(k) > k.
 * https://oeis.org/A348932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with k == 1 or 5 (mod 6) such that A348930(k) > k,
 *            where A348930(k) = sigma(k) with all factors of 3 removed; found
 *            by search
 * Range:     u8 n <= 32, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A348932_sigma(uint64_t n)
{
    uint64_t s = 1;
    for (uint64_t p = 2; p * p <= n; p += (p == 2 ? 1 : 2)) {
        if (n % p == 0) {
            uint64_t term = 1, pw = 1;
            while (n % p == 0) {
                n /= p;
                pw *= p;
                term += pw;
            }
            s *= term;
        }
    }
    if (n > 1) s *= (n + 1);
    return s;
}

seq_t a(idx_t n)
{
    uint64_t k = 1;
    idx_t count = 0;
    int step = 0; /* k=1 -> next 5 (+4), then +2 */
    for (;;) {
        uint64_t s = A348932_sigma(k);
        while (s % 3 == 0) s /= 3;
        if (s > k) {
            ++count;
            if (count == n) return (seq_t)k;
        }
        if (k == 1) k = 5;
        else if (step == 0) { k += 2; step = 1; }
        else { k += 4; step = 0; }
    }
}
