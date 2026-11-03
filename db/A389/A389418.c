/*
 * A389418  Least integer k such that Sum_{j=k..k+n-1} j is the concatenation
 *          of two consecutive integers, x and x - 1. No leading zeros
 *          allowed.
 * https://oeis.org/A389418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 87, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9754 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements OEIS A389418:
   a(n) = least integer k such that
         Σ_{j=k}^{k+n-1} j = concat(x, x‑1)
   for some positive integer x (no leading zeros).  The sequence is
   1‑based. */

#include <stdbool.h>
#include <stdint.h>

/* Return true iff S is the decimal concatenation of two consecutive
   positive integers x and x‑1.  No leading zeros are allowed. */
static bool is_concat(seq_t S)
{
    /* The concatenation must have at least two digits, thus S ≥ 10. */
    if (S < 10) return false;

    seq_t pow10 = 10;                /* current 10^d (d ≥ 1) */
    while (pow10 <= S) {
        /* If S = y * (10^d + 1) + 10^d,
           then y = x‑1 and the concatenation is x‖(x‑1). */
        seq_t denom = pow10 + 1;      /* 10^d + 1 */
        seq_t rem   = S - pow10;      /* S – 10^d  (≥ 0) */

        if (rem % denom == 0) {
            seq_t y = rem / denom;    /* y = x‑1 */
            seq_t low  = pow10 / 10;  /* 10^{d‑1}  – lower bound for y */
            seq_t high = pow10 - 1;   /* 10^d – 1   – upper bound for y */

            if (y >= low && y <= high) {
                /* y has exactly d digits ⇒ concatenation has no leading zeros. */
                return true;
            }
        }
        pow10 = pow10 * 10;           /* advance to next power of ten */
    }
    return false;
}

/* Find the least k such that the sum of n consecutive integers starting
   at k is a concatenation of two consecutive integers. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* fall‑back for out‑of‑range calls */

    /* The sum of k … k+n‑1 equals n*k + n*(n‑1)/2. */
    seq_t constBase = ((seq_t)n * ((seq_t)n - 1)) / 2;

    idx_t k = 1;
    while (1) {
        seq_t S = (seq_t)n * (seq_t)k + constBase;
        if (is_concat(S))
            return (seq_t)k;        /* cast to seq_t for consistency */
        ++k;
    }
}
