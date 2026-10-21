/*
 * A279015  Greatest possible number of diagonals of a polyhedron having n
 *          faces.
 * https://oeis.org/A279015
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2 - 21*n + 64 for n = 12 or n >= 14; special values
 *            for 4 <= n <= 13 as listed below: a(4)=0, a(5)=0, a(6)=4,
 *            a(7)=10, a(8)=20, a(9)=34, a(10)=52, a(11)=73, a(12)=100,
 *            a(13)=128
 * Range:     u8 n <= 16, u16 n <= 186, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset (not used) */
    if (n < 4) return (seq_t)0;

    /* Handle the first few special cases explicitly */
    switch ((uint64_t)n) {
    case 4: case 5: return (seq_t)0;
    case 6: return (seq_t)4;
    case 7: return (seq_t)10;
    case 8: return (seq_t)20;
    case 9: return (seq_t)34;
    case 10:return (seq_t)52;
    case 11:return (seq_t)73;
    case 12:return (seq_t)100;
    case 13:return (seq_t)128;
    default:
        /* For n >= 14 use the closed‑form expression */
        {
            seq_t nn = (seq_t)n;
            seq_t part = (nn * nn) * 2;   /* 2 * n^2  */
            part += (seq_t)64;            /* + 64    */
            part -= 21 * nn;              /* - 21 * n*/
            return part;
        }
    }
}
