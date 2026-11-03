/*
 * A256504  Summative Fission - For a positive integer n, find the greatest
 *          number of consecutive positive integers (at least 2) which add to
 *          n. For each of these do the same ... iterate to completion. a(n) =
 *          the total number of integers (including n itself) defined.
 * https://oeis.org/A256504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 333, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9333 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A256504: Summative Fission
   a(0) = 0
   For n > 0 let L be the greatest integer ≥ 2 for which n can be
   written as a sum of L consecutive positive integers.  If no such L
   exists a(n) = 1.  Otherwise a(n) = 1 + Σ a(x_i) where the x_i are
   the consecutive summands.  This implementation evaluates a(n)
   recursively without memoisation, using only integer arithmetic. */

#include <stddef.h>   /* for idx_t */

/* Helper: returns the largest ℓ ≥ 2 such that n can be expressed as
   a sum of ℓ consecutive positive integers, or 0 if no such ℓ exists. */
static idx_t best_split(idx_t n)
{
    if (n < 2) return 0;   /* n = 0 or 1 can never be split */
    /* Find maximal ℓ satisfying ℓ(ℓ+1)/2 ≤ n.  This ℓ is at most
       sqrt(2n), so a simple loop is fast for n ≤ 10000. */
    idx_t ℓ = 1;
    while ((ℓ + 1) * (ℓ + 2) / 2 <= n) ++ℓ;  /* ℓ is now the maximal value */

    for (idx_t l = ℓ; l >= 2; --l) {
        idx_t offset = l * (l - 1) / 2;          /* sum of first l‑1 integers */
        if (offset >= n) continue;              /* would give non‑positive start */
        idx_t rem = n - offset;
        if (rem % l == 0) return l;             /* valid decomposition found */
    }
    return 0;                                   /* no decomposition of length ≥2 */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* definition of the sequence */
    if (n == 1) return 1;   /* a(1) = 1 */

    idx_t L = best_split(n);
    if (L == 0) return 1;   /* cannot be split further */

    /* Starting integer of the consecutive block */
    idx_t offset = L * (L - 1) / 2;
    idx_t start  = (n - offset) / L;  /* start ≥ 1 by construction */

    seq_t sum = 0;
    for (idx_t x = start; x < start + L; ++x)
        sum += a(x);

    return 1 + sum;   /* count n itself and all descendants */
}
