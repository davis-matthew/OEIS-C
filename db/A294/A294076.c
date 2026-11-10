/*
 * A294076  Absolute difference between n-th stella octangula number (A007588)
 *          and the nearest perfect power (A001597).
 * https://oeis.org/A294076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min over perfect powers P of |n*(2*n^2-1) - P|; computed
 *            by taking, for each exponent k, the floor k-th root r of
 *            A007588(n) and testing r^k and (r+1)^k
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 60 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* true if b^k <= x (no overflow) */
static bool A294076_powle(seq_t b, int k, seq_t x) {
    seq_t r = 1;
    for (int i = 0; i < k; i++) {
        if (r > x / b) return false;
        r *= b;
    }
    return true;
}

static seq_t A294076_pw(seq_t b, int k) {
    seq_t r = 1;
    for (int i = 0; i < k; i++) r *= b;
    return r;
}

seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    seq_t S = m * (2 * m * m - 1);
    if (S == 0) return (seq_t)1;
    int bits = 0;
    for (seq_t t = S; t > 0; t >>= 1) bits++;
    seq_t best = S - 1;  /* candidate perfect power 1 */
    for (int k = 2; k <= bits; k++) {
        seq_t lo = 1, hi = (seq_t)1 << (bits / k + 1);
        while (lo < hi) {
            seq_t mid = lo + (hi - lo + 1) / 2;
            if (A294076_powle(mid, k, S)) lo = mid;
            else hi = mid - 1;
        }
        seq_t low = A294076_pw(lo, k);
        seq_t d = S - low;
        if (d < best) best = d;
        if (best == 0) return (seq_t)0;
        seq_t cap = S + best;
        if (A294076_powle(lo + 1, k, cap)) {
            seq_t up = A294076_pw(lo + 1, k);
            d = up - S;
            if (d < best) best = d;
        }
    }
    return best;
}
