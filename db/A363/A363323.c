/*
 * A363323  Total number of parts coprime to n in the partitions of n into 5
 *          parts.
 * https://oeis.org/A363323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{x=1}^{n-4}
 *            (p4(n-x)+p3(n-2x)+p2(n-3x)+p1(n-4x)+[n==5x]) * [gcd(n,x)=1]
 * Range:     u8 n <= 18, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where p_k(m) is the number of partitions of m into exactly k parts (p_1(m)=1 for m>=1). */

#include <stdbool.h>
#include "../utils/gcd.h"

#define MAXN 10000

static seq_t p[6][MAXN + 1];
static bool p_init = false;

static void precompute(void)
{
    if (p_init)
        return;

    /* initialize all to 0 */
    for (idx_t k = 0; k <= 5; ++k)
        for (idx_t i = 0; i <= MAXN; ++i)
            p[k][i] = 0;

    p[0][0] = 1;          /* partition of 0 into 0 parts */
    for (idx_t i = 1; i <= MAXN; ++i)
        p[1][i] = 1;      /* exactly one part */

    /* compute p_k for k = 2..5 using recurrence p_k(n) = p_k(n-k) + p_{k-1}(n-1) */
    for (idx_t k = 2; k <= 5; ++k) {
        for (idx_t n = 0; n <= MAXN; ++n) {
            if (n < k)
                p[k][n] = 0;
            else {
                seq_t a = p[k][n - k];
                seq_t b = p[k - 1][n - 1];
                p[k][n] = a + b;
            }
        }
    }

    p_init = true;
}

seq_t a(idx_t n)
{
    precompute();

    if (n < 5)
        return 0;

    seq_t sum = 0;

    for (idx_t x = 1; x <= n - 4; ++x) {
        /* skip if x is not coprime to n */
        if (gcd((seq_t)n, (seq_t)x) != 1)
            continue;

        idx_t m1 = n - x;
        idx_t m2 = n - 2 * x;
        idx_t m3 = n - 3 * x;
        idx_t m4 = n - 4 * x;

        seq_t occ = 0;
        /* contributions from p_k */
        if (m1 <= MAXN)
            occ += p[4][m1];
        if (m2 <= MAXN)
            occ += p[3][m2];
        if (m3 <= MAXN)
            occ += p[2][m3];
        if (m4 <= MAXN)
            occ += p[1][m4];
        /* the term [n == 5x] */
        if (n == 5 * x)
            occ += 1;

        sum += occ;
    }

    return sum;
}
