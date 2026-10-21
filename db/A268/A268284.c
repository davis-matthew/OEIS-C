/*
 * A268284  Period 15: repeat {18, 20, 28, 20, 20, 22, 18, 22, 20, 16, 12, 22,
 *          18, 40, 18}.
 * https://oeis.org/A268284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18 + offset(n mod 15) where * offset(0)=0, offset(1)=2,
 *            offset(2)=10, offset(3)=2, offset(4)=2, * offset(5)=4,
 *            offset(6)=0, offset(7)=4, offset(8)=2, offset(9)=-2, *
 *            offset(10)=-6, offset(11)=4, offset(12)=0, offset(13)=22,
 *            offset(14)=0 * (periodic definition of A268284)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 15;
    seq_t v = 18;
    switch (r) {
        case 1: case 3: case 4: case 8:
            v += 2; break;
        case 2:
            v += 10; break;
        case 5: case 7: case 11:
            v += 4; break;
        case 9:
            v -= 2; break;
        case 10:
            v -= 6; break;
        case 13:
            v += 22; break;
        default:
            /* r == 0, 6, 12, 14: v stays 18 */
            break;
    }
    return v;
}
