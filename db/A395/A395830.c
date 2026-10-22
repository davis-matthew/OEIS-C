/*
 * A395830  Number of hexadecimal strings of length n that contain a specific
 *          string xx (where x is a single digit).
 * https://oeis.org/A395830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16^n – (a_n + b_n) where a_n is the number of strings of
 *            length n ending in a non‑x digit and avoiding the pattern xx,
 *            and b_n is the number of such strings ending in the digit x (but
 *            the previous character is not x). Recurrence: a_{n+1} = 15·(a_n
 *            + b_n) b_{n+1} = a_n With a_0 = 1, b_0 = 0...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* States of the automaton */
    seq_t a_ = (seq_t)1;   /* strings ending with a non‑x digit */
    seq_t b_ = (seq_t)0;   /* strings ending with an x digit (previous not x) */
    seq_t pow16 = (seq_t)1;  /* 16^n */

    for (idx_t i = (idx_t)0; i < n; ++i)
    {
        /* Update 16^i */
        pow16 *= (seq_t)16;

        /* Transition to next length */
        seq_t new_a = (seq_t)15 * (a_ + b_);
        seq_t new_b = a_;

        a_ = new_a;
        b_ = new_b;
    }

    seq_t avoid = a_ + b_;
    return pow16 - avoid;
}
