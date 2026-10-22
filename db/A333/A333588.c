/*
 * A333588  a(n) = floor(-(3/2)*a(n-1)), a(1)=-2.
 * https://oeis.org/A333588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 24, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = -2; a(n) = floor(-(3/2)*a(n-1)) */
seq_t a(idx_t n)
{
    /* The sequence is not defined for n < 1; return 0 as a placeholder. */
    if (n == 0) return 0;

    seq_t val = -2;
    if (n == 1) return val;

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t mag = (val < 0) ? -val : val;             /* |a(n‑1)| */

        if (val < 0)  /* prev negative → next positive, floor(3*mag/2) */
        {
            seq_t next = (mag * 3) / 2;                 /* floor division */
            val = next;
        }
        else          /* prev positive → next negative, ceil(3*mag/2) */
        {
            seq_t next = (mag * 3 + 1) / 2;             /* ceil division */
            val = -next;
        }
    }

    return val;
}
