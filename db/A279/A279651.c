/*
 * A279651  Number of n X 2 0..2 arrays with no element equal to a strict
 *          majority of its horizontal and antidiagonal neighbors, with the
 *          exception of exactly two elements, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A279651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A279651
    a(n) = 24*a(n-1) – 204*a(n-2) + 704*a(n-3) – 816*a(n-4)
           + 384*a(n-5) – 64*a(n-6)    for n > 7
    Initial values (n = 1 … 7):
    1, 3, 24, 232, 2232, 20880, 190656
*/

seq_t a(idx_t n)
{
    /* Term for n < 1 is undefined; return 0 for safety. */
    if (n == 0) return (seq_t)0;

    /* The first seven terms (indexed from 1). */
    static const seq_t init[7] = {
        (seq_t)1,     (seq_t)3,     (seq_t)24,
        (seq_t)232,   (seq_t)2232,  (seq_t)20880,
        (seq_t)190656
    };

    if (n <= 7) return init[n - 1];

    /* Keep the last six computed terms in a sliding window: */
    seq_t a1 = init[1];   /* a(2) */
    seq_t a2 = init[2];   /* a(3) */
    seq_t a3 = init[3];   /* a(4) */
    seq_t a4 = init[4];   /* a(5) */
    seq_t a5 = init[5];   /* a(6) */
    seq_t a6 = init[6];   /* a(7) */

    for (idx_t i = 8; i <= n; ++i) {
        /* Compute a(i) from the recurrence. */
        seq_t next =
            (seq_t)24 * a6
          - (seq_t)204 * a5
          + (seq_t)704 * a4
          - (seq_t)816 * a3
          + (seq_t)384 * a2
          - (seq_t)64  * a1;

        /* Shift the window for the next iteration. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = next;
    }

    return a6;
}
