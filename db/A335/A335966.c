/*
 * A335966  a(n) is the number of odd terms in the n-th row of triangle
 *          A056939.
 * https://oeis.org/A335966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of odd terms in the n-th row of triangle A056939.
 *            Computation is based on the parity of the binomial product
 *            T(n+1,k) = binom(n+2,k-1)*binom(n+2,k)*binom(n+2,k+1) /
 *            (binom(n+2,1)*binom(n+2,2)) using 2-adic valuations. The
 *            implementation runs in O(n) time and uses O(n) auxiliary memory
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   _tz
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/_tz.h"

seq_t a(idx_t n)
{
    /* Map sequence index n to triangle row index m = n + 1
       and auxiliary value a_val = m + 1 = n + 2. */
    const idx_t m = n + 1;          /* triangle row index (0‑based) */
    const idx_t a_val = m + 1;      /* a = m + 1 */

    /* Allocate array for binary digit sums up to a_val. */
    unsigned char *S = (unsigned char*)malloc(a_val + 1);
    if (!S) return 0;  /* safety: out of memory (unlikely for n ≤ 10000) */

    S[0] = 0;
    for (idx_t i = 1; i <= a_val; ++i) {
        S[i] = S[i >> 1] + (unsigned char)(i & 1);
    }

    /* 2‑adic valuations */
    const idx_t v2a = _tz(a_val);
    const idx_t v2m = _tz(m);

    /* Target sum for parity condition */
    const int target_sum = 3 * S[a_val] + 2 * (int)v2a + (int)v2m - 1;

    seq_t count = 0;
    for (idx_t k = 1; k <= m; ++k) {
        unsigned char s1 = S[k - 1];
        unsigned char s2 = S[k];
        unsigned char s3 = S[k + 1];
        unsigned char t1 = S[a_val - (k - 1)];
        unsigned char t2 = S[a_val - k];
        unsigned char t3 = S[a_val - (k + 1)];
        int sum = (int)s1 + (int)s2 + (int)s3 +
                  (int)t1 + (int)t2 + (int)t3;
        if (sum == target_sum)
            ++count;
    }

    free(S);
    return count;
}
