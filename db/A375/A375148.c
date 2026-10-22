/*
 * A375148  Expansion of Sum_{k in Z} x^k / (1 - x^(7*k+2)).
 * https://oeis.org/A375148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A375148
 *  Formula: a(n) = sum_{k≥0} [ (n-2k) mod (7k+1) == 0 ]  –  sum_{k'≥1} [ (n+2k') mod (7k'-1) == 0 ]
 *  (k=0 contributes one for all n≥0).  This is equivalent to the coefficient of
 *  xⁿ in the generating function:  Σ_{k∈ℤ} x^(2k)/(1-x^(7k+1)).
 *
 *  The function below evaluates a(n) exactly using only integer arithmetic.
 *  It works for any idx_t n ≥ 0 and any seq_t that represents an integer
 *  (unsigned or arbitrary‑precision).  All intermediate counts are small
 *  (≤ n), so they fit comfortably in seq_t.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Count positive‑k contributions (including k = 0). */
    seq_t pos = 1;                     /* k = 0 term */
    idx_t max_pos_k = n / 2;           /* since 2k ≤ n for a non‑empty term */
    for (idx_t k = 1; k <= max_pos_k; ++k) {
        idx_t denom = 7 * k + 1;       /* 7k+1 > 0 */
        idx_t diff  = n - 2 * k;       /* n‑2k ≥ 0 by loop bound */
        if (diff % denom == 0) {
            ++pos;
        }
    }

    /* Count negative‑k contributions (k' = -k > 0). */
    seq_t neg = 0;
    /* For k' ≥ 1 we need 5k'‑1 ≤ n ⇒ k' ≤ (n+1)/5. */
    idx_t max_neg_kp = (n + 1) / 5;
    for (idx_t kp = 1; kp <= max_neg_kp; ++kp) {
        idx_t denom = 7 * kp - 1;      /* 7k'‑1 > 0 (kp ≥ 1) */
        idx_t sum   = n + 2 * kp;      /* n+2k' ≥ 0 */
        if (sum % denom == 0) {
            ++neg;
        }
    }

    /* pos ≥ neg because the sequence is non‑negative. */
    return pos - neg;
}
