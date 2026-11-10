/*
 * A286162  Compound filter: a(n) = T(A001511(n), A278222(n)), where T(n,k) is
 *          sequence A000027 used as a pairing function.
 * https://oeis.org/A286162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(A001511(n), A278222(n)), T(k,m) = (2 + (k+m)^2 - k -
 *            3m)/2; A001511(n) = 2-adic valuation of 2n; A278222(n) = least
 *            number whose prime signature is the multiset of lengths of the
 *            runs of 1's in binary n
 * Range:     u8 n <= 20, u16 n <= 170, u32 n <= 5460, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9182 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A286162_ruler(idx_t n) {
    seq_t r = 1;
    while ((n & 1) == 0) { r++; n >>= 1; }
    return r;
}

static seq_t A286162_runs(idx_t n) {
    static const unsigned pr[32] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,
                                    59,61,67,71,73,79,83,89,97,101,103,107,109,113,127,131};
    int cnt[65];
    int i, len = 0, idx = 0, L;
    for (i = 0; i <= 64; i++) cnt[i] = 0;
    for (i = 0; i < 64; i++) {
        if ((n >> i) & 1) len++;
        else { if (len) cnt[len]++; len = 0; }
    }
    if (len) cnt[len]++;
    seq_t m = 1;
    for (L = 64; L >= 1; L--) {
        int c;
        for (c = 0; c < cnt[L]; c++) {
            int e;
            for (e = 0; e < L; e++) m *= (seq_t)pr[idx];
            idx++;
        }
    }
    return m;
}

seq_t a(idx_t n) {
    seq_t k = A286162_ruler(n);
    seq_t m = A286162_runs(n);
    seq_t s = k + m;
    return (s * s + 2 - k - 3 * m) / 2;
}
