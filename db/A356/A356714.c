/*
 * A356714  Cardinality of the set{a_1+a_2+a_3+a_4: -floor((n-1)/2) <=
 *          a_1,a_2,a_3,a_4 <= floor(n/2), and a_1^2,a_2^2,a_3^2,a_4^2 are
 *          pairwise distinct}.
 * https://oeis.org/A356714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n - 15 for n >= 9. For n <= 8 the values are obtained
 *            by brute‑force enumeration of all 4‑tuples (a1,a2,a3,a4) in the
 *            allowed range with pairwise distinct squares
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Small n: enumerate directly.  */
    if (n <= 8) {
        int L = -(int)((n - 1) / 2);
        int R = (int)(n / 2);
        int minSum = 4 * L;
        int maxSum = 4 * R;
        int size   = maxSum - minSum + 1;

        /* visited array large enough for the sum range.  */
        static unsigned char visited[200];
        memset(visited, 0, sizeof(visited));

        int count = 0;
        for (int a1 = L; a1 <= R; ++a1) {
            int s1 = a1 * a1;
            for (int a2 = L; a2 <= R; ++a2) {
                int s2 = a2 * a2;
                if (s1 == s2) continue;
                for (int a3 = L; a3 <= R; ++a3) {
                    int s3 = a3 * a3;
                    if (s1 == s3 || s2 == s3) continue;
                    for (int a4 = L; a4 <= R; ++a4) {
                        int s4 = a4 * a4;
                        if (s1 == s4 || s2 == s4 || s3 == s4) continue;
                        int sum = a1 + a2 + a3 + a4;
                        int idx  = sum - minSum;       /* 0 … size-1   */
                        if (!visited[idx]) {
                            visited[idx] = 1;
                            ++count;
                        }
                    }
                }
            }
        }
        return (seq_t)count;
    }

    /* Large n: use the proven linear formula.  */
    return (seq_t)((seq_t)4 * n - 15);
}
