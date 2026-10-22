/*
 * A335408  Diameter of nearest neighbor interchange distance for free
 *          3-trees.
 * https://oeis.org/A335408
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = 3*n - 15 for n ≥ 9; a(3)=0, a(4)=1, a(5)=3, a(6)=5,
 *            a(7)=7, a(8)=10, a(9)=12 (look‑up table otherwise)
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t _lookup[6] = {0, 1, 3, 5, 7, 10};

seq_t a(idx_t n)
{
    /* For n < 3 the function is not specified; behaviour is undefined.  */
    if (n <= 8) {
        return _lookup[n - 3];
    }
    /* For n ≥ 9 the linear formula holds.  */
    return ((seq_t)n * 3) - (seq_t)15;
}
