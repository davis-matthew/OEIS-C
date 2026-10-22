/*
 * A301851  Table read by antidiagonals: T(n, k) gives the number of distinct
 *          distances on an n X k pegboard.
 * https://oeis.org/A301851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of distinct distances on an i × j pegboard where
 *            (i,j) is the n‑th pair in antidiagonal reading. The pair is
 *            obtained by solving m*(m+1)/2 ≥ n with m = i+j–2, then i = n –
 *            (m–1)*m/2 and j = m+1–i
 * Range:     u8 n <= 832, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define MAX_SUM 40000                /* (142‑1)^2 + (142‑1)^2 < 40000 */

/* the marked array stores, for each possible sum d^2+e^2, the
   last counter value in which it was seen */
static unsigned int vis[MAX_SUM + 1] = {0};
static unsigned int counter = 0;         /* global counter for marking */

/* number of distinct sums d^2+e^2 with 0≤d<i, 0≤e<j */
static inline seq_t distinct_sums(idx_t i, idx_t j)
{
    seq_t count = 0;
    unsigned int local = ++counter;
    if (local == 0) { /* wrap‑around protection, should never happen for our constraints */
        for (int k = 0; k <= MAX_SUM; ++k) vis[k] = 0;
        local = 1;
        counter = 1;
    }
    for (idx_t di = 0; di < i; ++di) {
        unsigned int di2 = (unsigned int)di * (unsigned int)di;
        for (idx_t dj = 0; dj < j; ++dj) {
            unsigned int dj2 = (unsigned int)dj * (unsigned int)dj;
            unsigned int s = di2 + dj2;
            if (vis[s] != local) {
                vis[s] = local;
                ++count;
            }
        }
    }
    return count;
}

seq_t a(idx_t n)
{
    /* Find m satisfying m*(m+1)/2 >= n , m>=1.
       Use binary search on 64‑bit indices, safe from overflow via __int128. */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;            /* m = i + j - 2 */
    idx_t d = m + 1;          /* i + j = d */
    idx_t prev_total = (m > 0) ? ((unsigned __int128)(m - 1) * m / 2) : 0;
    idx_t offset = n - prev_total;   /* 1‑based position on this diagonal */
    idx_t i = offset;
    idx_t j = d - offset;     /* ensures i>=1 and j>=1 */

    return distinct_sums(i, j);
}
