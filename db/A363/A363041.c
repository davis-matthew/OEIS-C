/*
 * A363041  Triangle read by rows: T(n,k) = Stirling2(n+1,k)/binomial(k+1,2)
 *          if n-k is even, else 0 (1 <= k <= n).
 * https://oeis.org/A363041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 48, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A363041
   Formula:  a(n) = 0                     if (r - k) is odd (where r = ⌈(√(8n+1)-1)/2⌉,
           a(n) = Stirling2(r+1,k) / binom(k+1,2) otherwise,
           with n = (r-1)r/2 + k , 1 ≤ k ≤ r.
           The triangle T(n,k) = Stirling2(n+1,k) / binom(k+1,2) when n-k is even,
           else 0.  */

/* Includes allowed for both C11 and C++17 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------------- */
/*  Helper: compute Stirling numbers of the second kind up to row n needed   */
/*  for a single query.  The function builds the DP table up to the target  */
/*  row on demand, reusing a static cache between calls.  It works for any   */
/*  seq_t provided the basic arithmetic operators are defined.              */
/* ------------------------------------------------------------------------- */

static seq_t **stirling_cache = NULL;   /* stirling_cache[n] points to a row array of size n+1 */
static idx_t  stirling_max   = 0;        /* largest n computed so far                            */

/* Ensure cache contains rows up to target_n (inclusive). */
static void
ensure_stirling(idx_t target_n)
{
    if (target_n <= stirling_max) return;

    /* Allocate or grow array of pointers to rows. */
    seq_t **new_cache = (seq_t**)realloc(stirling_cache,
                                         sizeof(seq_t*) * (target_n + 1));
    if (!new_cache) abort();
    stirling_cache = new_cache;

    /* Allocate and compute missing rows. */
    for (idx_t n = stirling_max + 1; n <= target_n; ++n) {
        /* Row n needs indices 0..n. */
        seq_t *row = (seq_t*)malloc(sizeof(seq_t) * (n + 1));
        if (!row) abort();
        stirling_cache[n] = row;

        if (n == 0) { /* S(0,0)=1 */
            row[0] = (seq_t)1;
            continue;
        }

        /* By definition S(n,0)=0 for n>0. */
        row[0] = (seq_t)0;
        seq_t *prev = stirling_cache[n - 1];

        /* Fill entries for k=1..n. */
        for (idx_t k = 1; k <= n; ++k) {
            if (k == n) {            /* S(n,n)=1 */
                row[k] = (seq_t)1;
            } else {
                /* S(n,k) = k*S(n-1,k) + S(n-1,k-1) */
                row[k] = (seq_t)k * prev[k] + prev[k - 1];
            }
        }
    }
    stirling_max = target_n;
}

/* ------------------------------------------------------------------------- */
/*  Main sequence function                                                     */
/* ------------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    /* Determine the triangle row r and column k for the index n.
       r is the smallest integer such that r(r+1)/2 ≥ n. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    idx_t prev_cum = (r - 1) * r / 2; /* cumulative count before row r */
    idx_t k = n - prev_cum;           /* 1 ≤ k ≤ r */

    /* Parity check: if (r-k) is odd, the entry is 0. */
    if (((r - k) & 1UL) != 0) return (seq_t)0;

    /* Need Stirling2(r+1, k). */
    ensure_stirling(r + 1);
    seq_t S_val = stirling_cache[r + 1][k];

    /* Compute denominator binom(k+1, 2) = (k+1)*k/2. */
    seq_t denom = ((seq_t)(k + 1) * (seq_t)k) / (seq_t)2;

    return S_val / denom;
}
