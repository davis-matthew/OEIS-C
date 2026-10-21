/*
 * A259078  Heptagonal numbers (A000566) that are other heptagonal numbers
 *          divided by 2.
 * https://oeis.org/A259078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 * Sequence A259078: Heptagonal numbers that are half another heptagonal number.
 *
 * The indices m of the smaller heptagonal numbers satisfy
 *   m*(5m-3) = 2 n*(5n-3),
 * which can be transformed into the Pell equation
 *   X² - 2 Y² = -9    with  X = 10n-3,  Y = 10m-3.
 *
 * Starting from the fundamental solution that gives the first term
 *   X₀ = 717,  Y₀ = 507  (corresponding to m₀ = 51, n₀ = 72),
 * subsequent solutions are obtained by multiplying by the sixth power
 * of the fundamental unit (3+2√2)²³ = 19601 + 13860√2.
 *
 * Recurrence for each step (k → k+1):
 *     X_{k+1} = 19601·X_k + 2·13860·Y_k
 *     Y_{k+1} = 13860·X_k + 19601·Y_k
 *
 * From Y_k we recover the index of the smaller heptagonal number:
 *     m_k = (Y_k + 3) / 10
 *
 * The term itself is the corresponding heptagonal number:
 *     a_k = m_k · (5·m_k - 3) / 2
 *
 * This implementation uses only integer arithmetic and works for
 * all seq_t / idx_t types provided by the build system.  It
 * doubles the indices with every iteration, so an O(n) algorithm
 * easily meets the 2‑second requirement for n up to about 10 000.
 */

seq_t a(idx_t n)
{
    /* Constants (all fit in a native integer) */
    const seq_t U   = (seq_t)19601;   /* coefficient of X in X_next          */
    const seq_t V   = (seq_t)13860;   /* coefficient of Y in X_next and X in Y_next */
    const seq_t twoV = (seq_t)(2 * 13860); /* 2*V, used in X_next                */

    /* Start from the fundamental solution (k = 0). */
    seq_t X = (seq_t)717;   /* 10*n_k - 3   */
    seq_t Y = (seq_t)507;   /* 10*m_k - 3   */

    /* Iterate n-1 times to reach the nth term. */
    for (idx_t i = 1; i < n; ++i) {
        seq_t newX = U * X + twoV * Y;
        seq_t newY = V * X + U * Y;
        X = newX;
        Y = newY;
    }

    /* Recover the index m of the smaller heptagonal number. */
    seq_t m = (Y + (seq_t)3) / (seq_t)10;

    /* Compute the heptagonal number: a = m*(5*m-3)/2. */
    seq_t a_val = m * ((seq_t)5 * m - (seq_t)3) / (seq_t)2;
    return a_val;
}
