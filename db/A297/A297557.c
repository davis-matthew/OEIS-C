/*
 * A297557  Number of maximum independent vertex sets in the n-triangular grid
 *          graph.
 * https://oeis.org/A297557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = a(n-3) for n >= 12, with base values a(0)=1, a(1)=3,
 *            a(2)=1, a(3)=1, a(4)=1, a(5)=27, a(6)=2, a(7)=28, a(8)=32,
 *            a(9)=1, a(10)=27, a(11)=27. For n >= 12 this yields a(n)=1 if
 *            n%3==0, otherwise a(n)=27
 * Range:     u8 n <= 37, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 12) {
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)3;
            case 2: return (seq_t)1;
            case 3: return (seq_t)1;
            case 4: return (seq_t)1;
            case 5: return (seq_t)27;
            case 6: return (seq_t)2;
            case 7: return (seq_t)28;
            case 8: return (seq_t)32;
            case 9: return (seq_t)1;
            case 10: return (seq_t)27;
            case 11: return (seq_t)27;
        }
    }
    if (n % 3 == 0) return (seq_t)1;
    return (seq_t)27;
}
