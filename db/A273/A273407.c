/*
 * A273407  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 673", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=3; for n>=1, a(n)=8*n + (8-(-1)^n)
 * Range:     u8 n <= 30, u16 n <= 127, u32 n <= 127, u64 n <= 127,
 *            u128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)3;
    }
    seq_t nn = (seq_t)n;
    seq_t res = nn * 8 + 8;             /* 8*n + 8 */
    /*   8 - (-1)^n  equals  7 for even n, 9 for odd n. */
    if ((n & 1ULL) == 0) {              /* even */
        res -= 1;                      /* 7 = 8-1 */
    } else {                            /* odd */
        res += 1;                      /* 9 = 8+1 */
    }
    return res;
}
