/*
 * A351705  For any nonnegative number n with binary expansion Sum_{k >= 0}
 *          b_k * 2^k, a(n) is the numerator of d(n) = Sum_{k >= 0} b_k *
 *          2^A130472(k). See A351706 for the denominators.
 * https://oeis.org/A351705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, frac
 * Formula:   a(n) is the numerator of d(n) = Σ 2^{(-1)^k * ⌊(k+1)/2⌋} over
 *            all * bit positions k where n has a 1. The fraction is reduced
 *            to lowest terms
 * Range:     u8 n <= 383, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* Store exponents for set bits */
    int exps[64];
    int cnt = 0;
    uint64_t t = n;
    while (t) {
        int k = __builtin_ctzll(t);
        t &= t - 1;

        int exp = ((k & 1) ? -1 : 1) * ((k + 1) / 2); /* e(k) */
        exps[cnt++] = exp;
    }

    int min_e = exps[0];
    for (int i = 1; i < cnt; ++i)
        if (exps[i] < min_e) min_e = exps[i];

    /* Sum scaled integers */
    seq_t sum = 0;
    if (min_e < 0) {
        for (int i = 0; i < cnt; ++i) {
            int shift = exps[i] - min_e;         /* non‑negative */
            sum += ((seq_t)1 << shift);
        }
        /* Remove common powers of two (reduce the fraction) */
        while ((sum & 1) == 0 && sum != 0)
            sum >>= 1;
        return sum;
    } else {
        for (int i = 0; i < cnt; ++i) {
            sum += ((seq_t)1 << exps[i]);
        }
        return sum;
    }
}
