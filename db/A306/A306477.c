/*
 * A306477  Number of ways to write n as C(w+2,2) + C(x+3,4) + C(y+5,6) +
 *          C(z+7,8) with w,x,y,z nonnegative integers, where C(m,k) denotes
 *          the binomial coefficient m!/(k!*(m-k)!).
 * https://oeis.org/A306477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n =
 *            C(w+2,2)+C(x+3,4)+C(y+5,6)+C(z+7,8) with w,x,y,z >= 0
 * Range:     u8 n <= 169, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, but return 0 for 0 */

    seq_t count = 0;

    const seq_t N = (seq_t)n;      /* cast once for comparisons */

    for (idx_t w = 0;; ++w) {
        seq_t t2 = ((seq_t)(w + 2) * (w + 1)) / 2;    /* C(w+2,2) */
        if (t2 > N) break;

        for (idx_t x = 0;; ++x) {
            seq_t t4 = ((seq_t)(x + 3) * (x + 2) * (x + 1) * x) / 24;
            seq_t s2 = t2 + t4;
            if (s2 > N) break;

            for (idx_t y = 0;; ++y) {
                seq_t t6 = ((seq_t)(y + 5) * (y + 4) * (y + 3) *
                            (y + 2) * (y + 1) * y) / 720;
                seq_t s3 = s2 + t6;
                if (s3 > N) break;

                for (idx_t z = 0;; ++z) {
                    seq_t t8 = ((seq_t)(z + 7) * (z + 6) * (z + 5) *
                                (z + 4) * (z + 3) * (z + 2) *
                                (z + 1) * z) / 40320;
                    seq_t total = s3 + t8;
                    if (total > N) break;
                    if (total == N) ++count;
                }
            }
        }
    }

    return count;
}
