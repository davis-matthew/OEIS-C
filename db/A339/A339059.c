/*
 * A339059  Number of compositions (ordered partitions) of n into distinct
 *          parts congruent to 1 mod 4.
 * https://oeis.org/A339059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=1} k! * P((n-(2*k*k - k))/4 , k) with a(0)=1
 *            where P(m,k) is the number of partitions of m into parts ≤ k
 * Range:     u8 n <= 55, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define D_MAX 2600   /* maximum D = (n - (2*k^2 - k))/4 for n <= 10000 */
#define K_MAX 80     /* maximum number of parts needed for n <= 10000 */

/* pre‑computed tables */
static seq_t P[D_MAX + 1][K_MAX + 1];
static seq_t fact[K_MAX + 1];
static bool initialized = false;

/* initialize partition and factorial tables */
static void init_tables(void)
{
    if (initialized) return;

    size_t kmax = 70;          /* 2*k*k - k <= 10000  => k <= 70  */
    size_t dmax = 2500;        /* (10000)/4  */

    for (size_t k = 0; k <= kmax; ++k) P[0][k] = 1;
    for (size_t m = 1; m <= dmax; ++m) P[m][0] = 0;

    for (size_t m = 1; m <= dmax; ++m)
        for (size_t k = 1; k <= kmax; ++k)
            if (m >= k)
                P[m][k] = P[m][k-1] + P[m-k][k];
            else
                P[m][k] = P[m][k-1];

    fact[0] = 1;
    for (size_t i = 1; i <= kmax; ++i)
        fact[i] = fact[i-1] * (seq_t)i;

    initialized = true;
}

/* sequence function */
seq_t a(idx_t n)
{
    init_tables();

    if (n == 0) return 1;

    seq_t ans = 0;
    size_t kmax = 0;
    while ( (2ull*kmax*kmax - kmax) <= n ) ++kmax;   /* kmax may be <= 70 */

    for (size_t k = 1; k <= kmax; ++k)
    {
        idx_t min_sum = 2ull*k*k - k;
        if (min_sum > n) break;

        idx_t rem = n - min_sum;
        if (rem & 3ULL) continue;          /* not divisible by 4 */

        size_t D = (size_t)(rem >> 2);      /* (n-(2k^2-k))/4 */
        if (D > D_MAX) continue;            /* safety guard */

        seq_t part = P[D][k];
        seq_t contrib = fact[k] * part;
        ans += contrib;
    }

    return ans;
}
