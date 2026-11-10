/*
 * A257773  Number of numbers k, 0 <= k <= 9, such that n is a Belgian-k
 *          number.
 * https://oeis.org/A257773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k in 0..9 such that n is a Belgian-k number,
 *            i.e. n appears in the sequence that starts at k and repeatedly
 *            adds n's decimal digits cyclically; computed directly by
 *            checking n - k - prefix_sum_r = 0 mod digit_sum(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9867 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    unsigned long long v = (unsigned long long)n;
    if (v == 0) return (seq_t)1;
    int dig[24];
    int d = 0;
    unsigned long long t = v;
    while (t > 0) {
        dig[d++] = (int)(t % 10);
        t /= 10;
    }
    /* dig is least-significant first; reverse to most-significant first */
    int msd[24];
    int s = 0;
    for (int i = 0; i < d; i++) {
        msd[i] = dig[d - 1 - i];
        s += msd[i];
    }
    int count = 0;
    for (int k = 0; k <= 9; k++) {
        int pref = 0;
        int hit = 0;
        for (int r = 0; r < d && !hit; r++) {
            long long diff = (long long)v - k - pref;
            if (diff >= 0 && (diff % s) == 0) hit = 1;
            pref += msd[r];
        }
        count += hit;
    }
    return (seq_t)count;
}
