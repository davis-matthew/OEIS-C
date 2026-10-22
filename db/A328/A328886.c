/*
 * A328886  Squares that end in 444.
 * https://oeis.org/A328886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 262, u64 n <= 400, u128 n <= 400,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula implemented: a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3) - a(n-4) + a(n-5)
   (with initial values a(1) = 1444, a(2) = 213444, a(3) = 289444,
    a(4) = 925444, a(5) = 1077444). This recurrence generates the
   square numbers that end in 444 (OEIS A328886). */
seq_t a(idx_t n)
{
    /* Any index below the offset returns 0 – behavior is irrelevant. */
    if (n == 0) return (seq_t)0;

    /* Return the five pre‑computed base cases directly. */
    if (n == 1) return (seq_t)1444;
    if (n == 2) return (seq_t)213444;
    if (n == 3) return (seq_t)289444;
    if (n == 4) return (seq_t)925444;
    if (n == 5) return (seq_t)1077444;

    /* Iterate from the 6th term to n using the recurrence.  Keep the
       last five computed terms in a sliding window. */
    seq_t a1 = (seq_t)1444;       /* a(n-5)   */
    seq_t a2 = (seq_t)213444;     /* a(n-4)   */
    seq_t a3 = (seq_t)289444;     /* a(n-3)   */
    seq_t a4 = (seq_t)925444;     /* a(n-2)   */
    seq_t a5 = (seq_t)1077444;    /* a(n-1)   */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = a5
                     + (seq_t)2 * a4
                     - (seq_t)2 * a3
                     - a2
                     + a1;

        /* Shift the window forward */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }

    return a5;
}
