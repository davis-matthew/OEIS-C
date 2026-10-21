/*
 * A245094  Total squares count in n-th generation of Pythagoras tree
 *          variation which is rhombitrihexagonal tiling.
 * https://oeis.org/A245094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*((-1)^n + 6*n - 5)/4 for n > 5 Initial values: a(0)=1,
 *            a(1)=2, a(2)=4, a(3)=8, a(4)=13, a(5)=20
 * Range:     u8 n <= 57, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[6] = {1, 2, 4, 8, 13, 20};
    if (n < 6)
        return init[n];

    /* For n > 5 use the closed form:
       • even n (n = 2k): a(n) = 3 * (3k - 1)
       • odd  n (n = 2k+1): a(n) = 9 * k      */
    if ((n & 1u) == 0) {               /* even n */
        seq_t k = (seq_t)(n / 2);
        return 3 * (3 * k - 1);
    } else {
        seq_t k = (seq_t)((n - 1) / 2);
        return 9 * k;
    }
}
