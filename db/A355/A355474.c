/*
 * A355474  Square array T(m,n) = Card({ (i, j) : 1 <= i <= m, 1 <= j <=
 *          min(n, i), GCD(i, j) = 1 }), read by antidiagonals upwards.
 * https://oeis.org/A355474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) counts coprime pairs (i,j) with 1 ≤ i ≤ m, 1 ≤ j ≤ min(n,i)
 *            where (m,n) are the coordinates of the n‑th element read along
 *            the antidiagonals of the square array T(m,n) = |{(i,j) : 1≤i≤m,
 *            1≤j≤min(n,i), gcd(i,j)=1}|. This implementation finds (m,n)
 *            using triangular numbers and enumerates the c...
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t compute_T(idx_t m, idx_t n)
{
    seq_t count = 0;
    for (idx_t i = 1; i <= m; ++i) {
        idx_t maxj = (n < i) ? n : i;
        for (idx_t j = 1; j <= maxj; ++j) {
            if (gcd((seq_t)i, (seq_t)j) == 1)
                count++;
        }
    }
    return count;
}

seq_t a(idx_t n)
{
    /* Find the anti‑diagonal d that contains index n */
    idx_t d = 0;
    idx_t sum = 0;
    while (sum < n) {
        d++;
        sum += d;
    }
    idx_t prev_sum = sum - d;
    idx_t offset = n - prev_sum - 1;          /* 0‑based offset in diagonal */
    idx_t m = d - offset;                     /* row coordinate */
    idx_t k = offset + 1;                     /* column coordinate */

    return compute_T(m, k);
}
