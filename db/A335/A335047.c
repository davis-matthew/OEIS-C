/*
 * A335047  Maximum sum of primes (see Comments).
 * https://oeis.org/A335047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)-3 for even n ≥ 4, a(n) = n*(n+1)-4 for odd n ≥ 7.
 *            Special cases: a(1)=0, a(2)=3, a(3)=8, a(4)=17, a(5)=24, a(6)=37
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    if (n == 2) return 3;
    if (n == 3) return 8;
    if (n == 4) return 17;
    if (n == 5) return 24;
    if (n == 6) return 37;

    seq_t tn = (seq_t)n;
    tn = tn * (tn + 1);
    if ((n & 1) == 0)  /* even */
        tn -= 3;
    else                /* odd */
        tn -= 4;
    return tn;
}
