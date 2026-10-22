/*
 * A348006  Largest increment in the trajectory from n to 1 in the Collatz map
 *          (or 3x+1 problem), or -1 if no such trajectory exists.
 * https://oeis.org/A348006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = maximum increment in the Collatz trajectory of n. For a
 *            starting value n, simulate the Collatz map until reaching 1 and
 *            keep track of the largest increase y−x where y = 3·x+1 for an
 *            odd x. If n is a power of two, only halving steps occur, so the
 *            maximum increment is 0
 * Range:     u8 n <= 26, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t max_inc = 0;

    while (x != (seq_t)1) {
        if ((x % ((seq_t)2)) == (seq_t)1) {   /* odd step: 3x+1 */
            seq_t inc = ((seq_t)2) * x + (seq_t)1; /* (3x+1)−x */
            if (inc > max_inc) max_inc = inc;
            x = (seq_t)3 * x + (seq_t)1;
        } else {                              /* even step: x/2 */
            x = x / (seq_t)2;
        }
    }
    return max_inc;
}
