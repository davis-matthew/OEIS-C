/*
 * A303401  Number of ways to write n as a*(3*a-1)/2 + b*(3*b-1)/2 + 3^c + 3^d
 *          with a,b,c,d nonnegative integers.
 * https://oeis.org/A303401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered solutions of * n = P(a) + P(b) + 3^c
 *            + 3^d with 0 ≤ a ≤ b, * 0 ≤ c ≤ d, P(x) = x*(3*x-1)/2
 *            (pentagonal numbers)
 * Range:     u8 n <= 110, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: brute‑force over the small ranges of a,b,c,d
 *  (a,b appear in O(sqrt(n)) values and c,d in a few powers of 3).
 *  The routine runs in roughly 1.5·10^5 elementary operations for
 *  n ≤ 10 000 and scales easily to larger n. */

#include <stddef.h>          /* for size_t */

seq_t a(idx_t n) {
    seq_t count = 0;                     /* result counter */

    /* ------- precompute powers of 3 ≤ n --------------------------- */
    seq_t pow[64];
    size_t powCnt = 0;
    seq_t powVal = 1;
    while (powVal <= n) {
        pow[ powCnt++ ] = powVal;
        /* stop before overflow (powVal * 3 would exceed n) */
        if (powVal > n / 3) break;
        powVal *= 3;
    }

    /* ------- loop over a,b (unordered, a ≤ b) -------------------- */
    for (idx_t a = 0; ; ++a) {
        seq_t aPent = (seq_t)a * ((seq_t)a * 3 - 1) / 2;
        if (aPent > n) break;            /* no larger a possible */

        for (idx_t b = a; ; ++b) {
            seq_t bPent = (seq_t)b * ((seq_t)b * 3 - 1) / 2;
            seq_t sumAB  = aPent + bPent;
            if (sumAB > n) break;        /* further b would only increase sum */
            seq_t remain = n - sumAB;     /* what must be represented by 3^c+3^d */

            /* ------- loop over c,d (unordered, c ≤ d) ---------------- */
            for (idx_t c = 0; c < powCnt; ++c) {
                seq_t valC = pow[c];
                if (valC > remain) break;
                for (idx_t d = c; d < powCnt; ++d) {
                    seq_t valD = pow[d];
                    seq_t eps = valC + valD;
                    if (eps > remain) break;
                    if (eps == remain) {
                        /* found a valid unordered quadruple (a,b,c,d) */
                        count++;
                    }
                }
            }
        }
    }
    return count;
}
