/*
 * A329588  Number of representative solutions (a, b) of the complex
 *          congruence z^2 == +1 (mod m) with z = a + b*i and nonvanishing
 *          a*b.
 * https://oeis.org/A329588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A227091(m) - A329586(m) where m = m(n) is the n-th
 *            modulus (m>=4) with nonzero solutions (all even m>=4 and the odd
 *            terms of A329589, in increasing order); found by scanning m
 *            upward and keeping those m with a positive difference
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 76 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A329588_stats(uint64_t m, seq_t *out)
{
    uint64_t t = m;
    int e2 = 0, r1 = 0, r3 = 0;
    while ((t & 1ULL) == 0) { e2++; t >>= 1; }
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            while (t % p == 0) t /= p;
            if (p % 4 == 1) r1++; else r3++;
        }
    }
    if (t > 1) {
        if (t % 4 == 1) r1++; else r3++;
    }
    int m2 = e2 < 3 ? e2 : 3;
    int r2 = (e2 >= 3) ? 2 : (e2 >= 2 ? 1 : 0);
    seq_t A = ((seq_t)1) << (m2 + 2 * r1 + r3);
    seq_t B = ((seq_t)1) << (r2 + r1 + r3);
    if (e2 <= 1 && r3 == 0)
        B += ((seq_t)1) << r1;
    if (A <= B) return false;
    *out = A - B;
    return true;
}

seq_t a(idx_t n)
{
    idx_t cnt = 0;
    for (uint64_t m = 4;; m++) {
        seq_t s;
        if (A329588_stats(m, &s)) {
            cnt++;
            if (cnt == n) return s;
        }
    }
}
