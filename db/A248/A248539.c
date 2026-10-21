/*
 * A248539  Number of length 2+3 0..n arrays with every four consecutive terms
 *          having the sum of some three elements equal to three times the
 *          fourth.
 * https://oeis.org/A248539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of length‑5 arrays a[0]..a[4] with 0≤a[i]≤n *
 *            such that for each window of four consecutive entries the sum of
 *            any * three equals three times the remaining one. * The sequence
 *            satisfies the empirical recurrence * * a(n)=
 *            a(n-1)-a(n-2)+a(n-4)+a(n-7)+a(n-8)-a(n-9)+a(n-10) * -a(n-...
 * Range:     u8 n <= 5, u16 n <= 29, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* helper to test if a 4‑tuple satisfies the condition */
static inline int tuple_good(int b,int c,int d,int x)
{
    int t = b + c + d;
    int S = t + x;
    if (S & 3) return 0;                /* S not divisible by 4 */
    int m = S >> 2;                     /* mean */
    return (b==m || c==m || d==m || x==m);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined, return 0 */

    /* allocate array 1..n (index 0 unused) */
    size_t size = (size_t)(n + 1);
    seq_t *seq = (seq_t *)malloc(size * sizeof(seq_t));
    if (!seq) return 0;                 /* allocation failed */

    /* compute initial values a(1) … a(28) by brute force when needed */
    idx_t max_small = (n < 28 ? n : 28);
    for (idx_t i = 1; i <= max_small; ++i) {
        seq[i] = 0;
        int max = (int)i;
        for (int a0 = 0; a0 <= max; ++a0)
            for (int a1 = 0; a1 <= max; ++a1)
                for (int a2 = 0; a2 <= max; ++a2)
                    for (int a3 = 0; a3 <= max; ++a3) {
                        if (!tuple_good(a0, a1, a2, a3)) continue;
                        for (int a4 = 0; a4 <= max; ++a4) {
                            if (tuple_good(a1, a2, a3, a4))
                                seq[i] = seq[i] + 1;
                        }
                    }
    }

    /* for n > 28 use the empirical recurrence */
    if (n > 28) {
        for (idx_t i = 29; i <= n; ++i) {
            /* sum of positive coefficients */
            seq_t pos = seq[i-1] + seq[i-4] + seq[i-7] + seq[i-8]
                    + seq[i-10] + seq[i-18] + seq[i-20] + seq[i-21]
                    + seq[i-24] + seq[i-27];
            /* sum of negative coefficients */
            seq_t neg = seq[i-2] + seq[i-9] + seq[i-12] + seq[i-13]
                    + seq[i-15] + seq[i-16] + seq[i-19] + seq[i-26]
                    + seq[i-28];
            seq[i] = pos - neg;   /* pos > neg for the sequence */
        }
    }

    seq_t result = seq[n];
    free(seq);
    return result;
}
