/*
 * A244352  a(n) = Pell(n)^3 - Pell(n)^2, where Pell(n) is the n-th Pell
 *          number (A000129).
 * https://oeis.org/A244352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Pell(n)^3 - Pell(n)^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th Pell number (Pell(0)=0, Pell(1)=1, Pell(n)=2*Pell(n-1)+Pell(n-2)) */
static seq_t pell(idx_t n) {
    seq_t a = 1, b = 0, c = 0, d = 1;          /* identity matrix I */
    seq_t ma = 2, mb = 1, mc = 1, md = 0;      /* base matrix M = [[2,1],[1,0]] */
    idx_t e = n;
    while (e) {
        if (e & 1) {
            seq_t ta = a * ma + b * mc;
            seq_t tb = a * mb + b * md;
            seq_t tc = c * ma + d * mc;
            seq_t td = c * mb + d * md;
            a = ta; b = tb; c = tc; d = td;
        }
        seq_t ma2 = ma * ma + mb * mc;
        seq_t mb2 = ma * mb + mb * md;
        seq_t mc2 = mc * ma + md * mc;
        seq_t md2 = mc * mb + md * md;
        ma = ma2; mb = mb2; mc = mc2; md = md2;
        e >>= 1;
    }
    return c;  /* Pell(n) is the element (1,0) of M^n */
}

seq_t a(idx_t n) {
    seq_t p  = pell(n);
    seq_t p2 = p * p;        /* Pell(n)^2 */
    seq_t p3 = p2 * p;       /* Pell(n)^3 */
    return p3 - p2;          /* Pell(n)^3 − Pell(n)^2 */
}
