/*
 * A322143  Square array A(n,k), n >= 1, k >= 0, read by antidiagonals: A(n,k)
 *          = Sum_{d|n, d==1 (mod 4)} d^k - Sum_{d|n, d==3 (mod 4)} d^k.
 * https://oeis.org/A322143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = Sum_{d|r, d≡1 (mod 4)} d^k - Sum_{d|r, d≡3 (mod 4)} d^k,
 *            where (r,k) are the coordinates of the antidiagonal that
 *            contains n
 * Range:     i8 n <= 30, i16 n <= 70, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n) {
    /* Find the diagonal s such that n lies on it: n <= s*(s+1)/2  */
    idx_t s = 1;
    for (;;) {
        __int128 tri = (__int128)s * (s + 1) / 2;
        if (tri >= n) break;
        ++s;
    }

    /* Compute (r,k) from the linear index n  */
    idx_t prev = (s - 1) * s / 2;   /* number of terms in previous diagonals */
    idx_t r    = n - prev;          /* 1 <= r <= s  (row index in A) */
    idx_t k    = s - r;             /* 0 <= k <= s-1 (column index in A) */

    /* Sum the contributions of divisors of r according to their residue mod 4 */
    seq_t pos = 0;  /* sum of d^k for d≡1 (mod 4) */
    seq_t neg = 0;  /* sum of d^k for d≡3 (mod 4) */

    for (idx_t d = 1; d * d <= r; ++d) {
        if (r % d == 0) {
            seq_t v1 = pow_seq((seq_t)d, (int)k);
            if (d % 4 == 1)      pos += v1;
            else if (d % 4 == 3) neg += v1;

            idx_t d2 = r / d;
            if (d2 != d) {
                seq_t v2 = pow_seq((seq_t)d2, (int)k);
                if (d2 % 4 == 1)   pos += v2;
                else if (d2 % 4 == 3) neg += v2;
            }
        }
    }

    return pos - neg;
}
