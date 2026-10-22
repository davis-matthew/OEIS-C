/*
 * A384633  Expansion of (1+x-2*x^2-2*x^3) / (1-6*x^2-4*x^3+2*x^4).
 * https://oeis.org/A384633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 45, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from (1+x-2*x^2-2*x^3)/(1-6*x^2-4*x^3+2*x^4):
   a(0)=1, a(1)=1, a(2)=4, a(3)=8
   a(n)=6*a(n-2)+4*a(n-3)-2*a(n-4)   for n>=4 */

seq_t a(idx_t n) {
    /* small base cases */
    static const seq_t base[4] = { 1, 1, 4, 8 };
    if (n < 4)
        return base[n];

    /* sliding window for a_{i-4}, a_{i-3}, a_{i-2}, a_{i-1} */
    seq_t a0 = base[0];      /* a_{i-4} */
    seq_t a1 = base[1];      /* a_{i-3} */
    seq_t a2 = base[2];      /* a_{i-2} */
    seq_t a3 = base[3];      /* a_{i-1} */
    seq_t cur = 0;

    for (idx_t i = 4; i <= n; ++i) {
        cur = 6 * a2 + 4 * a1 - 2 * a0;
        /* shift window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = cur;
    }
    return cur;
}
