/*
 * A341900  Partial sums of A005165.
 * https://oeis.org/A341900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! + a(n-2) for n ≥ 2 with a(0) = 0 and a(1) = 1. This
 *            recurrence follows from the given OEIS entry
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* prevprev holds a(i-2), prev holds a(i-1) */
    seq_t prevprev = (seq_t)0; /* a(0) */
    seq_t prev     = (seq_t)1; /* a(1) */

    /* fact holds i! as we iterate */
    seq_t fact = (seq_t)1;

    /* Iterate from i = 2 up to n, updating factorial
       and the recurrence a(i) = i! + a(i-2). */
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;          /* fact = i! */
        seq_t next = fact + prevprev; /* a(i)      */
        prevprev = prev;          /* shift window */
        prev = next;
    }

    /* After the loop, 'prev' contains a(n) */
    return prev;
}
