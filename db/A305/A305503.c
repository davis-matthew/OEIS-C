/*
 * A305503  Largest cardinality of subsets A of {0,1,...,n-1} with |A + A| >
 *          |A - A|.
 * https://oeis.org/A305503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, hard, nonn
 * Formula:   a(n) = n-7 for n > 15; a(15)=9; a(1..14)=0 Computation uses a
 *            direct formula; no search or sieve needed
 * Range:     u8 n <= 47, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t vals[15] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9
    };

    if (n == 0)
        return 0;
    if (n <= 15)
        return vals[n - 1];
    return (seq_t)(n - 7);
}
