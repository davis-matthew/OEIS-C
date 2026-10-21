/*
 * A245527  Number of compositions of n into parts 4 and 5 with at least one 4
 *          and one 5.
 * https://oeis.org/A245527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = t(n) - (n%4==0?1:0) - (n%5==0?1:0) where t(0)=1,
 *            t(1)=t(2)=t(3)=0, and t(n)=t(n-4)+t(n-5) for n>=4. a(0)=0
 * Range:     u8 n <= 44, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* Circular buffer for the last 5 values of t. */
    seq_t t[5] = {0};
    t[0] = 1;                 /* t(0) */

    seq_t cur;

    for (idx_t i = 1; i <= n; ++i) {
        if (i == 1 || i == 2 || i == 3) {
            cur = 0;
        } else if (i == 4) {
            cur = t[0];
        } else {
            cur = t[(i - 4) % 5] + t[(i - 5) % 5];
        }
        t[i % 5] = cur;
    }

    seq_t tn = t[n % 5];
    seq_t res = tn;
    if (n % 4 == 0) res -= 1;
    if (n % 5 == 0) res -= 1;
    return res;
}
