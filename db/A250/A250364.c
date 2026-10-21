/*
 * A250364  Number of length 6 arrays x(i), i=1..6 with x(i) in i..i+n and no
 *          value appearing more than 3 times.
 * https://oeis.org/A250364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^6 + 6*n^5 + 15*n^4 + 5*n^3 + 57*n^2 + 13*n + 1 for n >
 *            3
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 39, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle small n by brute‑force enumeration */
    if (n <= 3) {
        idx_t i;
        seq_t count = 0;

        for (i = 1; i <= n + 1; ++i) {
            for (idx_t v2 = 2; v2 <= n + 2; ++v2) {
                for (idx_t v3 = 3; v3 <= n + 3; ++v3) {
                    for (idx_t v4 = 4; v4 <= n + 4; ++v4) {
                        for (idx_t v5 = 5; v5 <= n + 5; ++v5) {
                            for (idx_t v6 = 6; v6 <= n + 6; ++v6) {
                                /* Count frequencies of values 1..n+6 (max 9) */
                                idx_t freq[10] = {0};
                                ++freq[i];
                                ++freq[v2];
                                ++freq[v3];
                                ++freq[v4];
                                ++freq[v5];
                                ++freq[v6];
                                if (freq[i] <= 3 &&
                                    freq[v2] <= 3 &&
                                    freq[v3] <= 3 &&
                                    freq[v4] <= 3 &&
                                    freq[v5] <= 3 &&
                                    freq[v6] <= 3) {
                                    ++count;
                                }
                            }
                        }
                    }
                }
            }
        }
        return count;
    }

    /* For n > 3 use the explicit polynomial */
    seq_t s = (seq_t)n;
    seq_t n2 = s * s;
    seq_t n3 = n2 * s;
    seq_t n4 = n3 * s;
    seq_t n5 = n4 * s;
    seq_t n6 = n5 * s;

    seq_t res = n6;                     /* n^6 */
    res += (seq_t)6 * n5;               /* + 6*n^5 */
    res += (seq_t)15 * n4;              /* + 15*n^4 */
    res += (seq_t)5 * n3;               /* + 5*n^3 */
    res += (seq_t)57 * n2;              /* + 57*n^2 */
    res += (seq_t)13 * s;               /* + 13*n */
    res += 1;                           /* + 1 */

    return res;
}
