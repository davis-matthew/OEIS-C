/*
 * A385366  a(n) = Sum_{permutations p of [n]} des(p^2), where des(p) is the
 *          number of descents of p.
 * https://oeis.org/A385366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n ≤ 2, otherwise a(n) = (n‑1)! * (n² – n – 4) / 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 440 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    if (n <= 2) return 0;          /* offset cases */

    seq_t fn   = factorial(n - 1);  /* (n-1)!        */
    seq_t nn   = (seq_t)n;          /* cast n into seq_t */
    seq_t factor = nn * nn - nn - 4;/* n² – n – 4    */
    seq_t res   = fn * factor;      /* product       */
    res /= 2;                       /* division by 2 */
    return res;
}
