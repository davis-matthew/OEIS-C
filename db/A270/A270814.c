/*
 * A270814  a(1)=0; thereafter a(2k)=k+a(k), a(2k+1)=6k+4+a(6k+4).
 * https://oeis.org/A270814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; a(2k)=k + a(k); a(2k+1)=6k+4 + a(6k+4)
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 9465, u64 n <= 9465,
 *            u128 n <= 9465, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(1)=0; if n even: a(n)=n/2 + a(n/2); if n odd: a(n)=3*n+1 + a(3*n+1) */
seq_t a(idx_t n) {
    if (n == 1) return 0;
    if ((n & 1) == 0) {               /* n is even */
        return (seq_t)(n / 2) + a(n / 2);
    } else {                          /* n is odd */
        idx_t m = 3 * n + 1;
        return (seq_t)m + a(m);
    }
}
