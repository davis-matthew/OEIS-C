/*
 * A322053  Number of decimal strings of length n that contain a specific
 *          string xx (where x is a single digit).
 * https://oeis.org/A322053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 10^n – b(n), where b(n) counts n‑digit decimal strings that
 * avoid the consecutive double digit xx.  Recurrence:
 *   b(0) = 1
 *   b(1) = 10
 *   b(n) = 9*b(n−1) + 9*b(n−2)   for n ≥ 2
 */
seq_t a(idx_t n)
{
    /* integers that work for any seq_t type */
    const seq_t nine = 9;
    const seq_t ten = 10;

    /* b(0) and b(1) */
    seq_t b_prev2 = 1;      /* b(0) */
    seq_t b_prev1 = ten;    /* b(1) */

    /* 10^n */
    seq_t pow10 = ten;      /* 10^1 */

    /* compute up to n */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t b_curr = nine * b_prev1 + nine * b_prev2;
        pow10          = pow10 * ten;   /* 10^i */
        b_prev2 = b_prev1;
        b_prev1 = b_curr;
    }

    /* a(n) = 10^n – b(n) */
    return pow10 - b_prev1;
}
