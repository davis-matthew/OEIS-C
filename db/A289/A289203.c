/*
 * A289203  Number of maximum independent vertex sets in the n X n knight
 *          graph.
 * https://oeis.org/A289203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=6; for n>4,
 *            a(n)=(((-1)^n)+3)/2
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)1;
        case 2: return (seq_t)1;
        case 3: return (seq_t)2;
        case 4: return (seq_t)6;
        default: /* n > 4 */
            return ((n & (idx_t)1) == 0) ? (seq_t)2 : (seq_t)1;
    }
}
