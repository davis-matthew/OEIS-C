/*
 * A303749  First differences of A302774; Number of terms in A303762 that have
 *          prime(n) as their largest prime factor (A006530).
 * https://oeis.org/A303749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A302774(n+1) - A302774(n). For n ≥ 1 a(1) = 1, a(2) = 2.
 *            For odd n ≥ 3 let k = (n+1)/2 and define b₁ = 1, b₂ = 4,
 *            thereafter b_k = 3·b_{k-1} + 4 (k ≥ 3). Then a(n) = b_k. For
 *            even n ≥ 4 the value is a(n) = b_{n/2} + 3
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)2;

    if ((n & (idx_t)1) == 0) {                 /* even n ≥ 4 */
        idx_t k = n / 2;
        seq_t b;
        if (k == 1)                     /* shouldn't happen for n≥4 */
            b = (seq_t)1;
        else if (k == 2)
            b = (seq_t)4;
        else {
            b = (seq_t)4;
            for (idx_t t = 3; t <= k; ++t)
                b = b * 3 + 4;
        }
        return b + 3;
    } else {                                   /* odd n ≥ 3 */
        idx_t k = (n + 1) / 2;
        seq_t b;
        if (k == 1)
            b = (seq_t)1;
        else if (k == 2)
            b = (seq_t)4;
        else {
            b = (seq_t)4;
            for (idx_t t = 3; t <= k; ++t)
                b = b * 3 + 4;
        }
        return b;
    }
}
