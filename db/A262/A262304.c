/*
 * A262304  Tail of sequence of lower halves of n: repeatedly apply
 *          x->floor(x/2) starting at n until reach one of 2,3,4; a(n) =
 *          whichever of 2,3,4 is reached.
 * https://oeis.org/A262304
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Tail of sequence of lower halves of n. For n >= 2, repeatedly apply
   floor(x/2) until x becomes 2, 3, or 4.  The reached value is returned. */
seq_t a(idx_t n)
{
    idx_t x = n;
    while (x > (idx_t)4) {
        x >>= 1;          /* floor divide by 2 */
    }
    return (seq_t)x;       /* x is one of {2,3,4} */
}
