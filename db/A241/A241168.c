/*
 * A241168  Triangle read by rows: T(n,k) (1 <= k <= n) = Steffensen's bracket
 *          function [n,n-k].
 * https://oeis.org/A241168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the value of triangle T(r,k) read by rows, where T(r,k) =
 *            [r,k] = Σ_{s=k}^{r-1} Stirling2(r,s+1) · s! / k!. The index n
 *            (n≥1) is converted to the row r and the position k (1‑based) by
 *            the usual triangular numbering
 * Range:     u8 n <= 16, u16 n <= 34, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* idx_t is provided by the build; stdint.h is harmless */

#define MAX_R 200      /* enough for n up to about 10 000 (row ≈ 141) */

/* Pre‑computed factorials and Stirling numbers of the second kind. */
static seq_t fact_arr[MAX_R + 1];
static seq_t stirling[MAX_R + 1][MAX_R + 1];
static int precomputed = 0;

/* One‑time initialization of the tables. */
static void init_tables(void)
{
    if (precomputed)
        return;

    /* factorial[0] = 1; factorial[i] = factorial[i-1] * i. */
    fact_arr[0] = 1;
    for (idx_t i = 1; i <= MAX_R; ++i)
        fact_arr[i] = fact_arr[i - 1] * (seq_t)i;

    /* Stirling numbers of the second kind:
       S(0,0)=1, S(n,0)=0 (n>0);  S(n,k)=k*S(n-1,k)+S(n-1,k-1). */
    for (idx_t n = 0; n <= MAX_R; ++n) {
        for (idx_t k = 0; k <= n; ++k) {
            if (n == 0 && k == 0)
                stirling[n][k] = 1;
            else if (k == 0)
                stirling[n][k] = 0;
            else
                stirling[n][k] = k * stirling[n - 1][k] + stirling[n - 1][k - 1];
        }
    }

    precomputed = 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n=0; return 0 for safety */

    init_tables();

    /* Find the row r such that r*(r+1)/2 ≥ n. */
    idx_t r = 1;
    while (((seq_t)r * (r + 1)) / 2 < n)
        ++r;

    /* Number of elements in all previous full rows. */
    idx_t prev = (idx_t)(((seq_t)(r - 1) * r) / 2);

    /* Position within the current row (1‑based). */
    idx_t pos = n - prev;          /* 1 ≤ pos ≤ r */

    /* Corresponding bracket index k = r - pos. */
    idx_t k = r - pos;

    /* Compute the bracket [r,k]. */
    seq_t result = 0;
    for (idx_t s = k; s <= r - 1; ++s) {
        seq_t term = stirling[r][s + 1] * fact_arr[s];
        term = term / fact_arr[k];  /* division is exact */
        result += term;
    }
    return result;
}
