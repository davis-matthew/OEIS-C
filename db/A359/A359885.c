/*
 * A359885  Number of 3-dimensional tilings of a 2 X 2 X 3n box using trominos
 *          (three 1 X 1 X 1 cubes).
 * https://oeis.org/A359885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 60*a(n-1) - 128*a(n-2) for n≥2, a(0)=1, a(1)=44
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)44;

    /* Iteratively compute the recurrence up to the requested index.
       We only need the two preceding values at each step. */
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)44;  /* a(1) */
    seq_t curr  = 0;          /* will hold a(i) for i = 2 .. n */

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = 60*a(i-1) - 128*a(i-2) */
        curr = (seq_t)60 * prev1 - (seq_t)128 * prev2;
        /* slide the window */
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
