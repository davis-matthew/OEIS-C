/*
 * A246959  Numbers of (undirected) Hamiltonian cycles in the n-Sierpiński
 *          gasket graph.
 * https://oeis.org/A246959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 1 for n = 1, 2
 *             a(3) = 8
 *             a(n) = (3 * a(n-1))^3 for n ≥ 4
 *
 *  The recurrence follows from a(n) = 8 * 12^((3^(n-2) - 3)/2).
 *  This implementation uses only basic operations on seq_t,
 *  which are defined for all supported integer widths and for
 *  arbitrary-precision integers.
 */
seq_t a(idx_t n)
{
    /* Small indices */
    if (n == 1 || n == 2)
        return (seq_t)1;
    if (n == 3)
        return (seq_t)8;

    /* n >= 4 : use the recurrence a(n) = (3 * a(n-1))^3 */
    seq_t result = (seq_t)8;            /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t t = result * (seq_t)3;    /* 3 * a(n-1) */
        /* cube t: t * t * t */
        result = t * t * t;
    }

    return result;
}
