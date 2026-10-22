/*
 * A344118  Triangle of numbers T(n,k) =
 *          floor((A089072(n,k)-A019538(n,k))/A019538(n,k)) read by rows,
 *          n>=1, 1<=k<=n.
 * https://oeis.org/A344118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = floor((k^r - k!*S(r,k)) / (k!*S(r,k))) where r = row of
 *            the number n in the triangle, k = column (1 ≤ k ≤ r) S(r,k) are
 *            Stirling numbers of the second kind
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 54, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   factorial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* utility prototypes */
#include "../utils/seq_pow.h"   /* seq_pow(seq_t base, idx_t exp) */
#include "../utils/factorial.h" /* factorial(idx_t n) */

#include <stdbool.h>

#define MAX_R 200   /* sufficient for n up to ~10 000 (row < 141) */

/* triangular numbers t[r] = r*(r+1)/2   (t[0] = 0) */
static idx_t t[MAX_R + 1];
/* factorials 0! … MAX_R!   (fact[0] = 1) */
static seq_t fact[MAX_R + 1];
/* Stirling numbers of the second kind S[n][k]   (n,k ≤ MAX_R) */
static seq_t S[MAX_R + 1][MAX_R + 1];
/* flag indicating that the tables have been initialised */
static bool tables_inited = false;

/* initialise the static tables once */
static void init_tables(void)
{
    if (tables_inited) return;

    /* triangular numbers */
    t[0] = 0;
    for (idx_t i = 1; i <= MAX_R; ++i)
        t[i] = t[i - 1] + i;

    /* factorials */
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= MAX_R; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* Stirling numbers of the second kind */
    S[0][0] = (seq_t)1;          /* S(0,0) = 1, others default to 0 */
    for (idx_t n = 1; n <= MAX_R; ++n) {
        for (idx_t k = 1; k <= n; ++k)
            S[n][k] = (seq_t)k * S[n - 1][k] + S[n - 1][k - 1];
    }

    tables_inited = true;
}

/* find the row r such that t[r-1] < n ≤ t[r]   (1‑based row) */
static idx_t find_row(idx_t n)
{
    idx_t lo = 1, hi = MAX_R;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (t[mid] >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)   /* safety guard; the sequence is offset 1 */
        return (seq_t)0;

    init_tables();

    idx_t r = find_row(n);
    idx_t prev = t[r - 1];
    idx_t k = n - prev;   /* column index, 1 ≤ k ≤ r */

    seq_t power = seq_pow((seq_t)k, r);          /* k^r */
    seq_t denom = fact[k] * S[r][k];
    seq_t numer = power - denom;

    /* floor division – numerator is non‑negative in this definition,
       but we use the standard truncating division of unsigned types.   */
    return numer / denom;
}
