/*
 * A372076  The sequence T_{3,1}(n,3).
 * https://oeis.org/A372076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 26, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=2,
   a(n)=3*a(n-1)-3*a(n-2)+4*a(n-3) for n>=3   */
/* This is equivalent to the given double‑sum formula for A372076. */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* Use three variables to iterate the recurrence. */
    seq_t a0 = (seq_t)0;  /* a(n-3) */
    seq_t a1 = (seq_t)1;  /* a(n-2) */
    seq_t a2 = (seq_t)2;  /* a(n-1) */
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i)
    {
        /* a(n) = 3*a(n-1) - 3*a(n-2) + 4*a(n-3) */
        ai = 3*a2 - 3*a1 + 4*a0;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
