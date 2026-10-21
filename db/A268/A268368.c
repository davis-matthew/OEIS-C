/*
 * A268368  An eventually quasi-quadratic sequence satisfying a
 *          Hofstadter-like recurrence.
 * https://oeis.org/A268368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=1, a(3)=0; for n>=4: if n%4==0 : a = 2*(n/4)^2 +
 *            2*(n/4) if n%4==1 : a = 4*(n/4) if n%4==2 : a = 4 if n%4==3 : a
 *            = 3
 * Range:     u8 n <= 43, u16 n <= 723, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1)
        return 0;
    if (n == 2)
        return 1;
    if (n == 3)
        return 0;
    seq_t q = (seq_t)(n / 4);          /* n/4 fits in seq_t for all practical n */
    switch ((unsigned)(n % 4)) {
        case 0:
            return 2 * q * q + 2 * q;
        case 1:
            return 4 * q;
        case 2:
            return 4;
        case 3:
            return 3;
    }
    return 0;   /* unreachable but keeps compiler happy */
}
