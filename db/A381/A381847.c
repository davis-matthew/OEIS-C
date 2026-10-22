/*
 * A381847  Number of 3 element sets of distinct integer sided rectangles that
 *          fill an n X n square.
 * https://oeis.org/A381847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 - 6n + 12)/12) + floor((n-1) * floor((n-1)/2))
 * Range:     u8 n <= 22, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This formula yields the number of 3‑element sets of distinct integer‑sided
   rectangles that tile an n×n square.  The first term counts the vertical / 
   horizontal slice arrangements, the second counts the orthogonal elbow
   arrangements. */

seq_t a(idx_t n)
{
    /* Part 1: slice arrangements (vertical or horizontal). */
    seq_t slice = 0;
    if (n >= 6) {
        seq_t nn = (seq_t)n;
        slice = (nn * nn - 6 * nn + 12) / 12;
    }

    /* Part 2: orthogonal elbow arrangements. */
    seq_t term = (seq_t)(n - 1);
    seq_t half;
    if (n & 1) {                     /* n is odd  -> (n-1)/2 */
        half = (seq_t)((n - 1) / 2);
    } else {                          /* n is even -> n/2 - 1 */
        half = (seq_t)(n / 2 - 1);
    }
    seq_t elbow = half * term;

    return slice + elbow;
}
