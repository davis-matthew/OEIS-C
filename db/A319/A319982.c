/*
 * A319982  a(n) is the number of integer partitions of n with largest part <=
 *          4 for which the index of the seaweed algebra formed by the integer
 *          partition paired with its weight is 0.
 * https://oeis.org/A319982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-4) for n>20; Initial terms:
 *            a(1)=1,a(2)=1,a(3)=1,a(4)=2,a(5)=3,a(6)=2,a(7)=3,a(8)=3,
 *            a(9)=4,a(10)=3,a(11)=3,a(12)=2,a(13)=4,a(14)=2,a(15)=3,a(16)=1,
 *            a(17)=4,a(18)=2,a(19)=3,a(20)=0. Equivalent periodicity for
 *            n>16: 1→4, 2→2, 3→3, 0→0 (mod 4)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n <= 20) {
        static const seq_t tbl[21] = {
            0, /* unused */
            (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)2,
            (seq_t)3, (seq_t)3, (seq_t)4, (seq_t)3, (seq_t)3, (seq_t)2,
            (seq_t)4, (seq_t)2, (seq_t)3, (seq_t)1,
            (seq_t)4, (seq_t)2, (seq_t)3, (seq_t)0
        };
        return tbl[n];
    } else {
        switch (n % 4) {
            case 1: return (seq_t)4;
            case 2: return (seq_t)2;
            case 3: return (seq_t)3;
            default: return (seq_t)0; /* case 0 */
        }
    }
}
