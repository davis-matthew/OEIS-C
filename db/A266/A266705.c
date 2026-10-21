/*
 * A266705  Coefficient of x^2 in minimal polynomial of the continued fraction
 *          [1^n,sqrt(5),1,1,1,...], where 1^n means n ones.
 * https://oeis.org/A266705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3) for n > 3, a(0) = 1, a(1) =
 *            11, a(2) = 11, a(3) = 45
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;
    if (n == 2) return (seq_t)11;
    if (n == 3) return (seq_t)45;

    seq_t p3 = (seq_t)11;  /* a(n-3) */
    seq_t p2 = (seq_t)11;  /* a(n-2) */
    seq_t p1 = (seq_t)45;  /* a(n-1) */
    seq_t cur = 0;

    for (idx_t i = 4; i <= n; ++i) {
        cur = (seq_t)2 * p1 + (seq_t)2 * p2 - p3;
        p3 = p2;
        p2 = p1;
        p1 = cur;
    }

    return cur;
}
