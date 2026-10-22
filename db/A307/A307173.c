/*
 * A307173  Second class of all proper positive solutions y2(n) = a(n) of the
 *          Pell equation x^2 - 7*y^2 = 9.
 * https://oeis.org/A307173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A307173: second class of solutions y₂(n) of x² – 7y² = 9.
   Recurrence (from generating function x*(1+4x)/(1–16x+x²)):
        a(1) = 1
        a(2) = 20
        a(n) = 16*a(n‑1) – a(n‑2)      for n ≥ 3
   The function works for any seq_t (unsigned integer or arbitrary‑precision)
   and n ≥ 1.  It runs in O(n) time.  */

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 20;

    seq_t prev2 = 1;   /* a(1) */
    seq_t prev1 = 20;  /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t curr = (seq_t)16 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
