/*
 * A385859  a(n) = Sum_{k=0..n} (C(n,k) mod 3)^2.
 * https://oeis.org/A385859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (C(n,k) mod 3)^2 Computation via base‑3
 *            digits: if n has k '1' digits and m '2' digits, then a(n) =
 *            2^(k-1) * (5·3^m - 3) for k>0, and a(n) = (5·3^m - 3)/2 for k =
 *            0
 * Range:     u8 n <= 133, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count digits 1 and 2 in base 3 representation of n. */
    idx_t temp = n;
    int k = 0, m = 0;
    while (temp > 0) {
        int d = temp % 3;
        if (d == 1) k++;
        else if (d == 2) m++;
        temp /= 3;
    }

    /* Compute 3^m. */
    seq_t pow3 = (seq_t)1;
    for (int i = 0; i < m; ++i) pow3 *= (seq_t)3;

    /* Compute (5·3^m - 3). */
    seq_t term = (seq_t)5 * pow3 - (seq_t)3;

    if (k == 0) {
        /* For k = 0, divide by 2. The result is always integer. */
        return term / (seq_t)2;
    } else {
        /* Compute 2^(k-1). */
        seq_t pow2 = (seq_t)1;
        for (int i = 0; i < k - 1; ++i) pow2 *= (seq_t)2;
        return pow2 * term;
    }
}
