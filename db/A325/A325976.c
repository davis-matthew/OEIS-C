/*
 * A325976  a(n) is the largest k <= n such that k and (2n-sigma(n)) [=
 *          A033879(n)] are relatively prime.
 * https://oeis.org/A325976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{k ≤ n : gcd(k, |2n - sigma(n)|) = 1}
 * Range:     u8 n <= 255, u16 n <= 9531, u32 n <= 9531, u64 n <= 9531,
 *            u128 n <= 9531, big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n) {
    /* Compute 2n and sigma(n) */
    seq_t two_n = ((seq_t)n << 1);
    seq_t s = sigma_of(n);

    /* Take absolute value of 2n - sigma(n) */
    seq_t tAbs;
    if (two_n > s) {
        tAbs = two_n - s;
    } else {
        tAbs = s - two_n;
    }

    /* Search downward for the largest k ≤ n coprime to tAbs */
    for (idx_t k = n; k > 0; --k) {
        if (gcd((seq_t)k, tAbs) == 1) {
            return (seq_t)k;
        }
    }
    /* Fallback – for n ≥ 1 this should never be reached */
    return 1;
}
