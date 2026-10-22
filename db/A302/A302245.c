/*
 * A302245  Maximum remainder of p*q divided by p+q with 0 < p <= q <= n.
 * https://oeis.org/A302245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n-2)-(-1)^n for n > 6; a(1)=1, a(2)=2, a(3)=3, a(4)=5,
 *            a(5)=7, a(6)=8
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 6) {
        switch (n) {
            case 1: return 1;
            case 2: return 2;
            case 3: return 3;
            case 4: return 5;
            case 5: return 7;
            case 6: return 8;
            default: return 0; /* unreachable */
        }
    }
    seq_t nn = (seq_t)n;
    seq_t res;
    if ((n & 1ULL) == 0) {          /* even n */
        res = 2 * (nn - 2) - 1;
    } else {                        /* odd n */
        res = 2 * (nn - 2) + 1;
    }
    return res;
}
