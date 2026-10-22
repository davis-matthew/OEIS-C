/*
 * A323294  Number of 3-uniform hypergraphs spanning n labeled vertices where
 *          every two edges have two vertices in common.
 * https://oeis.org/A323294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=0, a(2)=0, a(3)=1, a(4)=11; a(n)=n*(n-1)/2 for n>=5
 * Range:     u8 n <= 23, u16 n <= 362, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
        case 0: return 1;
        case 1: return 0;
        case 2: return 0;
        case 3: return 1;
        case 4: return 11;
        default:
            seq_t nn = (seq_t)n;
            return nn * (nn - 1) / 2;
    }
}
