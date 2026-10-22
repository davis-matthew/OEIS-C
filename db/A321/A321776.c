/*
 * A321776  Number of compositions of n into parts with distinct
 *          multiplicities and with exactly six parts.
 * https://oeis.org/A321776
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into exactly six parts with
 *            distinct multiplicities. The algorithm enumerates all admissible
 *            patterns of multiplicities and counts the admissible size
 *            assignments. The result is returned as an arbitrary-precision
 *            integer if required by the build. Complexity: O(n...
 * Range:     u8 n <= 12, u16 n <= 118, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include "../utils/count2.h"

static const seq_t fact[7] = { (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)6,
                               (seq_t)24, (seq_t)120, (seq_t)720 };

/* Count ordered 3‑tuples (s1<s2<s3) of distinct positive integers
   satisfying m1*s1 + m2*s2 + m3*s3 == n. */
static seq_t count3(idx_t n, int m1, int m2, int m3, seq_t coeff) {
    seq_t total = 0;
    for (idx_t s1 = 1;; ++s1) {
        idx_t part1 = (idx_t)m1 * s1;
        if (part1 >= n) break;
        idx_t rem1 = n - part1;
        if (rem1 <= (idx_t)m2 + (idx_t)m3) continue;   /* at least one for s2,s3 */
        for (idx_t s2 = s1 + 1;; ++s2) {
            idx_t part2 = (idx_t)m2 * s2;
            if (part2 >= rem1) break;
            idx_t rem2 = rem1 - part2;
            if (rem2 <= (idx_t)m3) continue;       /* at least one for s3 */
            if (rem2 % (idx_t)m3 != 0) continue;
            idx_t s3 = rem2 / (idx_t)m3;
            if (s3 > s2) total += coeff;
        }
    }
    return total;
}

seq_t a(idx_t n) {
    if (n < 6) return 0;              /* offset starts at 6 */

    seq_t result = 0;
    /* pattern {6} */
    if (n % 6 == 0) result += (seq_t)1;

    /* patterns of size 2 */
    result += count2(n, 5, 1, (seq_t)6);   /* {5,1} coeff = 6 */
    result += count2(n, 1, 5, (seq_t)6);   /* {1,5} */
    result += count2(n, 4, 2, (seq_t)15);  /* {4,2} coeff = 15 */
    result += count2(n, 2, 4, (seq_t)15);  /* {2,4} */

    /* patterns of size 3, coefficient 60 */
    result += count3(n, 3, 2, 1, (seq_t)60);
    result += count3(n, 3, 1, 2, (seq_t)60);
    result += count3(n, 2, 3, 1, (seq_t)60);
    result += count3(n, 2, 1, 3, (seq_t)60);
    result += count3(n, 1, 3, 2, (seq_t)60);
    result += count3(n, 1, 2, 3, (seq_t)60);

    return result;
}
