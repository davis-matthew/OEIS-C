/*
 * A370300  Number of chordless cycles in the complement of the n-Sierpinski
 *          tetrahedron graph.
 * https://oeis.org/A370300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (9*16^n - 219*4^n + 1296) / 8 for n ≥ 2; a(1) = 0
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: fast exponentiation for seq_t */
static seq_t _pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & (idx_t)1) result *= base;
        base *= base;
        exp >>= (idx_t)1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n <= (idx_t)1) return (seq_t)0;

    seq_t pow16 = _pow_seq((seq_t)16, n);
    seq_t termA  = (seq_t)9 * pow16;

    seq_t pow4  = _pow_seq((seq_t)4, n);
    seq_t termB = (seq_t)219 * pow4;

    seq_t numerator = termA + (seq_t)1296;
    numerator -= termB;           /* termA + 1296 ≥ termB for n ≥ 2 */

    return numerator / (seq_t)8;
}
