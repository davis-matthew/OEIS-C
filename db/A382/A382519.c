/*
 * A382519  Odd positive integers m such that phi(m) and phi(m+1) are both
 *          powers of 2.
 * https://oeis.org/A382519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 1 (n=1) a(n) = 3 (n=2) a(n) = 5 (n=3) a(n) =
 *            Π_{i=0}^{n-3} (2^(2^i)+1) (4 ≤ n ≤ 7) a(n) = 0 (n > 7), because
 *            the sequence is finite
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)5;
    if (n >= 4 && n <= 7)
    {
        /* Fermat numbers F0 … F4 : 3, 5, 17, 257, 65537 */
        static const seq_t fermat[5] = {(seq_t)3, (seq_t)5,
                                        (seq_t)17, (seq_t)257,
                                        (seq_t)65537};
        seq_t val = 1;
        for (idx_t i = 0; i < n - 2; ++i)
            val *= fermat[i];
        return val;
    }
    return (seq_t)0;   /* sequence ends after n = 7 */
}
