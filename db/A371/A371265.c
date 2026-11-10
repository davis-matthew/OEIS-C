/*
 * A371265  Irregular triangle T(n, k), n >= 0, read by rows; the n-th row
 *          lists the numbers m such that A371263(m) = n.
 * https://oeis.org/A371265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   T(n,.) lists, in increasing order, the m with A371263(m)=n; row
 *            0 = {0}, row n>0 has 2^(A005811(n)-1) terms (balanced-ternary
 *            strings with leading digit 1, run lengths = binary run lengths
 *            of n, adjacent runs differ). Computed by locating row then
 *            unranking
 * Range:     u8 n <= 174, u16 n <= 9841, u32 n <= 9841, u64 n <= 9841,
 *            u128 n <= 9841, big any n
 * Validated: standard, pass, 83 terms through n = 9492 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;
    uint64_t j = (uint64_t)n - 1;
    uint64_t r = 1;
    int lens[70];
    int k = 0;
    for (;;) {
        /* compute runs of r */
        int hb = 63;
        while (!((r >> hb) & 1)) hb--;
        k = 0;
        int cur = 1, cnt = 0;
        for (int b = hb; b >= 0; --b) {
            int bit = (int)((r >> b) & 1);
            if (bit == cur) cnt++;
            else { lens[k++] = cnt; cur = bit; cnt = 1; }
        }
        lens[k++] = cnt;
        uint64_t c = (uint64_t)1 << (k - 1);
        if (j < c) break;
        j -= c;
        r++;
    }
    seq_t m = 0;
    int prev = 1;
    for (int i = 0; i < k; ++i) {
        int d;
        if (i == 0) d = 1;
        else {
            int bit = (int)((j >> (k - 1 - i)) & 1);
            int lo, hi;
            if (prev == -1) { lo = 0; hi = 1; }
            else if (prev == 0) { lo = -1; hi = 1; }
            else { lo = -1; hi = 0; }
            d = bit ? hi : lo;
        }
        for (int t = 0; t < lens[i]; ++t) {
            m = m * 3;
            if (d == 1) m++;
            else if (d == -1) m--;
        }
        prev = d;
    }
    return m;
}
