/*
 * A308566  Number of ways to write n as w^2 + x*(x+1) + 4^y*5^z with w,x,y,z
 *          nonnegative integers.
 * https://oeis.org/A308566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations n = w² + x·(x+1) + 4ʸ·5ᶻ with
 *            non‑negative integers w, x, y, z
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Public interface required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)  /* undefined by OEIS, return 0 as safe default */
        return 0;

    /* Allocate and zero the array of counts of w² + x·(x+1).  Use
       malloc and explicit zeroing to work with arbitrary‑precision
       seq_t types. */
    seq_t *cnt = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!cnt)
        return 0;           /* allocation failure */

    for (size_t i = 0; i <= (size_t)n; ++i)
        cnt[i] = 0;

    /* Count all (w, x) such that w² + x·(x+1) ≤ n. */
    for (idx_t w = 0; w * w <= n; ++w) {
        idx_t w2 = w * w;
        for (idx_t x = 0; ; ++x) {
            idx_t t = x * (x + 1u);      /* x*(x+1) */
            idx_t sum = w2 + t;
            if (sum > n)
                break;
            cnt[sum] += 1;              /* one more pair (w,x) */
        }
    }

    /* Generate all values of t = 4ʸ·5ᶻ that are ≤ n.  The number is tiny. */
    idx_t tvals[200];
    size_t tcount = 0;

    for (idx_t pow4 = 1; pow4 <= n; ) {
        for (idx_t pow5 = 1; ; ) {
            idx_t t = pow4 * pow5;
            if (t > n)
                break;
            tvals[tcount++] = t;
            if (pow5 > n / 5u)
                break;
            pow5 *= 5u;
        }
        if (pow4 > n / 4u)
            break;
        pow4 *= 4u;
    }

    /* Sum the contributions.  For each t, the leftover part
       n‑t must be equal to some w² + x·(x+1). */
    seq_t result = 0;
    for (size_t i = 0; i < tcount; ++i) {
        idx_t t = tvals[i];
        if (t <= n)
            result += cnt[n - t];
    }

    free(cnt);
    return result;
}
