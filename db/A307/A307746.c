/*
 * A307746  Triangle read by rows, obtained by omitting all the 1's from the
 *          triangle in A307641 (except for the first row).
 * https://oeis.org/A307746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   Row(i) = {d|i, A014963(d) > 1} A014963(d), listed for increasing
 *            d; row 1 = {1}; row i>1 has A001222(i) terms (flattened by rows)
 * Range:     u8 n <= 674, u16 n <= 9135, u32 n <= 9135, u64 n <= 9135,
 *            u128 n <= 9135, big any n
 * Validated: standard, pass, 97 terms through n = 8450 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A307746_omega_big(idx_t n) {
    idx_t c = 0;
    for (idx_t p = 2; p <= n / p; ++p) {
        while (n % p == 0) { n /= p; ++c; }
    }
    if (n > 1) ++c;
    return c;
}

/* returns the prime p if d is a power of prime p (d>1), else 0 */
static idx_t A307746_ppbase(idx_t d) {
    idx_t spf = d;
    for (idx_t p = 2; p <= d / p; ++p) {
        if (d % p == 0) { spf = p; break; }
    }
    idx_t t = d;
    while (t % spf == 0) t /= spf;
    return t == 1 ? spf : 0;
}

seq_t a(idx_t n)
{
    idx_t k = n;
    idx_t row = 1;
    for (;;) {
        idx_t len = (row == 1) ? 1 : A307746_omega_big(row);
        if (k <= len) break;
        k -= len;
        ++row;
    }
    if (row == 1) return (seq_t)1;
    idx_t cnt = 0;
    for (idx_t d = 2; d <= row; ++d) {
        if (row % d != 0) continue;
        idx_t p = A307746_ppbase(d);
        if (p) {
            ++cnt;
            if (cnt == k) return (seq_t)p;
        }
    }
    return (seq_t)1;
}
