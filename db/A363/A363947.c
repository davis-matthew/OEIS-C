/*
 * A363947  Number of integer partitions of n with mean < 3/2.
 * https://oeis.org/A363947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) – q(n, floor(2n/3)) where p(n) is the total number
 *            of partitions of n and q(n, m) counts partitions of n with
 *            largest part ≤ m (equivalently, partitions into at most m parts)
 * Range:     u8 n <= 36, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* a(0) is defined as 0 */

    /* total partitions p(n) using parts 1..n */
    seq_t *p = (seq_t*)calloc(n + 1, sizeof(seq_t));
    if (!p) return 0;                     /* allocation failure fallback */
    p[0] = 1;
    for (idx_t part = 1; part <= n; ++part) {
        for (idx_t sum = part; sum <= n; ++sum) {
            p[sum] += p[sum - part];
        }
    }

    /* partitions with largest part ≤ m, i.e. partitions into at most m parts
       where m = floor(2n/3) */
    idx_t m = (2 * n) / 3;
    seq_t *q = (seq_t*)calloc(n + 1, sizeof(seq_t));
    if (!q) { free(p); return 0; }
    q[0] = 1;
    for (idx_t part = 1; part <= m; ++part) {
        for (idx_t sum = part; sum <= n; ++sum) {
            q[sum] += q[sum - part];
        }
    }

    seq_t result = p[n] - q[n];

    free(p);
    free(q);
    return result;
}
