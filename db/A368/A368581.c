/*
 * A368581  The sum of weights of nondegenerated monotone Bacher
 *          representations of n.
 * https://oeis.org/A368581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1}^{floor(n/2)} * Sum_{w|k, w^2≤k} * Sum_{y|n-k,
 *            y^2≤n-k} * max(1, 2*([w^2<k] + [y^2<n-k])) * with the additional
 *            condition k < y*w * * The implementation enumerates the divisors
 *            of k and n‑k for each k * and sums the corresponding weights. It
 *            runs in O(n·d(k)·d(n‑k)) * time, which...
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* maximum number of divisors for n ≤ 10 000 is 128, so a safe upper bound */
static const size_t MAX_DIVS = 256;

/* Fill 'arr' with all positive divisors of n.  The number of divisors
 * is returned in *cnt.  The caller must provide an array of at least
 * MAX_DIVS elements.  The order of divisors is unspecified. */
static void get_divisors(uint64_t n, uint64_t *arr, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            arr[c++] = d;
            uint64_t d2 = n / d;
            if (d2 != d && c < MAX_DIVS) {
                arr[c++] = d2;
            }
        }
    }
    *cnt = c;
}

/* Sum of weights of nondegenerated monotone Bacher representations of n. */
seq_t a(idx_t n)
{
    if (n < 2)   /* a(1) = 0 by definition */
        return (seq_t)0;

    uint64_t wdivs[MAX_DIVS], ydivs[MAX_DIVS];
    size_t wcnt, ycnt;
    uint64_t total = 0;          /* accumulate in uint64_t to avoid overflow */

    for (idx_t k = 1; k <= n / 2; ++k) {
        uint64_t nk = n - k;

        /* divisors of k */
        get_divisors((uint64_t)k, wdivs, &wcnt);
        /* divisors of n-k */
        get_divisors(nk, ydivs, &ycnt);

        for (size_t iw = 0; iw < wcnt; ++iw) {
            uint64_t w = wdivs[iw];
            uint64_t w_sq = w * w;
            if (w_sq > (uint64_t)k)
                continue;                      /* w must satisfy w^2 ≤ k   */
            bool w_cond = (w_sq < (uint64_t)k);  /* indicator [w^2 < k] */

            for (size_t iy = 0; iy < ycnt; ++iy) {
                uint64_t y = ydivs[iy];
                uint64_t y_sq = y * y;
                if (y_sq > nk)
                    continue;                      /* y must satisfy y^2 ≤ n-k */
                bool y_cond = (y_sq < nk);        /* indicator [y^2 < n-k] */

                if ((uint64_t)k < w * y) {        /* condition k < y*w   */
                    int sumcond = (w_cond ? 1 : 0) + (y_cond ? 1 : 0);
                    int weight = sumcond ? 2 * sumcond : 1;
                    total += (uint64_t)weight;
                }
            }
        }
    }

    return (seq_t)total;
}
