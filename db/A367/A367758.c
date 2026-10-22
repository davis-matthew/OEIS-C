/*
 * A367758  Least number of inequivalent cells in a polyomino with n cells.
 * https://oeis.org/A367758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(5)=2. For n≠1,5 let k = n/8, c = n%8 (0≤c≤7). If c==0
 *            or 1: a = k + c + 1 If c==2 or 6: a = 2*k + (c+2)/4 If c==3 or
 *            7: a = 2*k + (c+5)/4 If c==4: a = k + 1 If c==5: a = k + 3
 * Range:     u8 n <= 1018, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 5) return 2;

    idx_t k = n / 8;
    unsigned int c = (unsigned int)(n % 8);
    seq_t res;

    switch (c) {
        case 0:
        case 1:
            res = (seq_t)k + (seq_t)c + (seq_t)1;
            break;
        case 2:
        case 6:
            res = (seq_t)2 * (seq_t)k + (seq_t)((c + 2) / 4);
            break;
        case 3:
        case 7:
            res = (seq_t)2 * (seq_t)k + (seq_t)((c + 5) / 4);
            break;
        case 4:
            res = (seq_t)k + (seq_t)1;
            break;
        case 5:
            res = (seq_t)k + (seq_t)3;
            break;
        default:
            res = 0; /* unreachable */
            break;
    }
    return res;
}
