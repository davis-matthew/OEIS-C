/*
 * A358902  Number of integer compositions of n whose parts have weakly
 *          decreasing numbers of distinct prime factors (A001221).
 * https://oeis.org/A358902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer compositions of n whose parts have
 *            weakly decreasing numbers of distinct prime factors (A001221)
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 48, u64 n <= 96, u128 n <= 193,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The empty composition is the only composition of 0. */
    if (n == 0)
        return (seq_t)1;

    /* ------------------------------------------------------------
       1. Compute ω(k) = number of distinct prime factors for 1 ≤ k ≤ n.
       ------------------------------------------------------------ */
    int *omega = (int *)calloc((size_t)(n + 1), sizeof(int));
    if (!omega)
        return (seq_t)0;
    omega[0] = 0;
    if (n >= 1)
        omega[1] = 0;

    for (idx_t p = 2; p <= n; ++p)
    {
        if (omega[p] == 0)          /* p is prime */
        {
            for (idx_t m = p; m <= n; m += p)
                ++omega[m];
        }
    }

    /* Find the maximum ω within the range. */
    int max_w = 0;
    for (idx_t k = 2; k <= n; ++k)
        if (omega[k] > max_w)
            max_w = omega[k];

    /* Number of rows for each sum: (max_w + 1). */
    size_t row = (size_t)(max_w + 1);
    size_t total = (size_t)(n + 1) * row;

    /* ------------------------------------------------------------
       2. Allocate two‑dimensional arrays f and suff.
          f[sum][w]  – number of compositions of 'sum' that end
                       with a part having ω = w.
          suff[sum][w] – suffix sums over w of f[sum][w]
          (used to enforce the weakly decreasing ω condition).
       ------------------------------------------------------------ */
    seq_t *f = (seq_t *)calloc(total, sizeof(seq_t));
    seq_t *suff = (seq_t *)calloc(total, sizeof(seq_t));
    if (!f || !suff)
    {
        free(omega);
        free(f);
        free(suff);
        return (seq_t)0;
    }

    /* ------------------------------------------------------------
       3. Dynamic programming over all sums up to n.
       ------------------------------------------------------------ */
    for (idx_t sum = 1; sum <= n; ++sum)
    {
        size_t base_sum = (size_t)sum * row;

        /* Add contributions from all parts k ≤ sum. */
        for (idx_t k = 1; k <= sum; ++k)
        {
            int w = omega[k];
            if (w > max_w)           /* safety check */
                continue;

            size_t idx_f = base_sum + (size_t)w;

            if (k == sum)
            {
                /* The composition consisting of a single part k. */
                f[idx_f] += (seq_t)1;
            }
            else
            {
                /* Append part k (with ω = w) to any composition of (sum‑k)
                   whose last part has ω ≥ w.  suff[sum‑k][w] contains exactly
                   that number. */
                size_t idx_suff = (size_t)(sum - k) * row + (size_t)w;
                f[idx_f] += suff[idx_suff];
            }
        }

        /* Compute suffix sums for this sum to be reused by larger sums. */
        suff[base_sum + max_w] = f[base_sum + max_w];
        for (int w = max_w - 1; w >= 0; --w)
        {
            suff[base_sum + (size_t)w] =
                f[base_sum + (size_t)w] + suff[base_sum + (size_t)(w + 1)];
        }
    }

    /* ------------------------------------------------------------
       4. Sum over all ending ω to obtain a(n).
       ------------------------------------------------------------ */
    seq_t result = (seq_t)0;
    size_t base_n = (size_t)n * row;
    for (int w = 0; w <= max_w; ++w)
        result += f[base_n + (size_t)w];

    /* ------------------------------------------------------------
       5. Clean up and return.
       ------------------------------------------------------------ */
    free(omega);
    free(f);
    free(suff);
    return result;
}
