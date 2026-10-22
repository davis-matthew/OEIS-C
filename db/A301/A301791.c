/*
 * A301791  Number of 2Xn 0..1 arrays with every element equal to 0 or 1
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n‑1) – a(n‑3) for n > 5 Initial values: a(1)=2,
 *            a(2)=5, a(3)=6, a(4)=9, a(5)=14
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base values */
    static const seq_t base[5] = { (seq_t)2, (seq_t)5, (seq_t)6, (seq_t)9, (seq_t)14 };

    if (n <= 5)
        return base[n - 1];

    /* use a sliding window of the last three terms */
    seq_t prev3 = base[2];   /* a(n-3) */
    seq_t prev2 = base[3];   /* a(n-2) */
    seq_t prev1 = base[4];   /* a(n-1) */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t cur = (prev1 << 1) - prev3; /* a(i) = 2*a(i-1) - a(i-3) */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
