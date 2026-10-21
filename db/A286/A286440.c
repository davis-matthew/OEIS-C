/*
 * A286440  Number of ways to tile an n X n X n triangular area with five 2 X
 *          2 X 2 triangular tiles and an appropriate number (= n^2-20) of 1 X
 *          1 X 1 tiles.
 * https://oeis.org/A286440
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{j=0}^{10} p_j * C(n-6-j+10,10) where p = {546, 8058,
 *            32574, 10218, -26514, -4734, 14274, -3762, -456, -84, 120}
 *            Offset: 5 (a(5)=0). For n ≥ 6 the formula gives the correct
 *            value
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 16, u64 n <= 137,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 6) return (seq_t)0;   /* a(5)=0, a(n<5) undefined */

    /* coefficients p_j for j = 0..10  */
    static const int coeff[11] = {
        546, 8058, 32574, 10218, -26514, -4734,
        14274, -3762, -456, -84, 120
    };

    seq_t pos = (seq_t)0;   /* sum of positive contributions   */
    seq_t neg = (seq_t)0;   /* sum of negative contributions   */

    /* iterate through the 11 terms */
    for (int j = 0; j < 11; ++j) {
        /* n' = n - 6 - j + 10 = n + 4 - j   */
        idx_t nprime = n + 4 - (idx_t)j;
        if (nprime < 10) continue;   /* C(n',10)=0 for n'<10 */

        /* compute C(nprime,10) multiplicatively */
        seq_t bin = (seq_t)1;
        for (int i = 1; i <= 10; ++i) {
            bin = bin * (seq_t)(nprime - 10 + i);
            bin = bin / (seq_t)i;
        }

        int c = coeff[j];
        if (c >= 0) {
            pos += bin * (seq_t)c;
        } else {
            neg += bin * (seq_t)(-c);
        }
    }
    return pos - neg;
}
