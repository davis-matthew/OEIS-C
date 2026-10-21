/*
 * A259508  Number of (n+1) X (1+1) 0..1 arrays with each 2 X 2 subblock
 *          having clockwise pattern 0000 0001 0101 0111.
 * https://oeis.org/A259508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 41, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) + 3*a(n-2) - 2*a(n-3);
   Initial values: a(1)=11, a(2)=31, a(3)=87. */

seq_t a(idx_t n)
{
    /* Handle the first three indices directly. */
    if (n == 1) return (seq_t)11;
    if (n == 2) return (seq_t)31;
    if (n == 3) return (seq_t)87;

    seq_t a1 = (seq_t)11,   /* a(n-3) */
          a2 = (seq_t)31,   /* a(n-2) */
          a3 = (seq_t)87;   /* a(n-1) */
    seq_t next;
    idx_t i;

    /* Compute iteratively up to the requested index. */
    for (i = 4; i <= n; ++i) {
        next = 2 * a3;           /* 2*a(n-1) */
        next += 3 * a2;          /* +3*a(n-2) */
        next -= 2 * a1;          /* -2*a(n-3) */
        a1 = a2;                 /* shift window forward */
        a2 = a3;
        a3 = next;
    }

    return a3;   /* a(n) now stored in a3 */
}
