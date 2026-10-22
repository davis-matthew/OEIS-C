/*
 * A373167  Triangle read by rows: the exponential almost-Riordan array (
 *          1/(1-x) | exp(x^2), 2*x ).
 * https://oeis.org/A373167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,0) = n! T(n,k) = (n‑1)!/(k‑1)! * [x^(n‑1)]
 *            exp(x²)·(2x)^(k‑1) = (n‑1)!/(k‑1)! ÷ ((n‑k)/2)! · 2^(k‑1) (n–k
 *            even) The sequence a(n) is the triangle read by rows: a(0) = 1
 *            a(1) = 1, a(2) = 1, a(3) = 2, a(4) = 0, a(5) = 2, …
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Memoised factorials up to 199! – enough for all n up to 10 000 */
static bool fact_inited = false;
static seq_t fact[200];

static void init_fact(void) {
    if (!fact_inited) {
        fact[0] = (seq_t)1;
        for (idx_t i = 1; i < 200; ++i)
            fact[i] = fact[i-1] * (seq_t)i;
        fact_inited = true;
    }
}

/* Return a(n), where a is the triangle read by rows. */
seq_t a(idx_t n) {
    init_fact();

    /* Determine the row r that contains index n.
       The cumulative number of terms up to and including row r is
       S(r) = (r+1)(r+2)/2.  We find the first r with S(r) > n. */
    idx_t r = 0;
    while (((r + 1) * (r + 2) / 2) <= n)
        ++r;

    /* Index k within the row r */
    idx_t prev_cum = r * (r + 1) / 2;   /* first index of row r */
    idx_t k = n - prev_cum;             /* 0 ≤ k ≤ r */

    /* Compute T(r,k) as described. */
    if (k == 0)               /* T(r,0) = r! */
        return fact[r];

    /* If r < k the triangle entry does not exist; treat as 0. */
    if (k > r)
        return (seq_t)0;

    /* n-k must be even; otherwise entry is zero. */
    if (((r - k) & 1) == 1)
        return (seq_t)0;

    idx_t j = (r - k) / 2;   /* (r-k)/2 */

    /* Compute (r-1)! / (k-1)! / j!  then multiply by 2^(k-1). */
    seq_t res = fact[r-1];       /* (r-1)!          */
    res /= fact[k-1];             /* / (k-1)!        */
    res /= fact[j];               /* / j!            */

    /* Multiply by 2^(k-1). */
    for (idx_t e = 0; e < k-1; ++e)
        res *= (seq_t)2;

    return res;
}
