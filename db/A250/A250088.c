/*
 * A250088  Number of length 7+5 0..n arrays with every six consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining four terms.
 * https://oeis.org/A250088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (173/63)*n^9 + (103/3)*n^8 + 130*n^7 + 224*n^6 +
 *            (1162/5)*n^5 + (764/3)*n^4 + (17908/63)*n^3 + 160*n^2 +
 *            (123/5)*n + 1 (OEIS offset: a(1) = 1348)
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 122, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* common denominator for the rational coefficients */
    const unsigned long long div = 315ULL;          /* 63*5 = 105*3 = 5*63*3 etc. */

    /* For 64‑bit seq_t, use 128‑bit intermediate arithmetic
       to avoid overflow of the huge powers of n. */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        /* ---- 64‑bit implementation using __uint128_t intermediates ---- */
        __uint128_t nn = (unsigned long long)n;         /* n fits in 64 bits */
        __uint128_t pow[10];
        pow[0] = 1;
        for (int i = 1; i <= 9; ++i) pow[i] = pow[i-1] * nn;

        __uint128_t num =
            (__uint128_t)865  * pow[9]  +   /* 173/63 */
            (__uint128_t)10815 * pow[8]  +   /* 103/3  */
            (__uint128_t)40950 * pow[7]  +   /* 130    */
            (__uint128_t)70560 * pow[6]  +   /* 224    */
            (__uint128_t)73206 * pow[5]  +   /* 1162/5 */
            (__uint128_t)80220 * pow[4]  +   /* 764/3  */
            (__uint128_t)89540 * pow[3]  +   /* 17908/63 */
            (__uint128_t)50400 * pow[2]  +   /* 160    */
            (__uint128_t)7749  * pow[1]  +   /* 123/5  */
            (__uint128_t)div;                /* constant term (1*div) */

        seq_t result = (seq_t)(num / div);
        return result;
    } else {
        /* ---- arbitrary‑precision implementation using seq_t operators ---- */
        seq_t nn = (seq_t)n;                           /* cast to seq_t */
        seq_t pow[10];
        pow[0] = 1;
        for (int i = 1; i <= 9; ++i) pow[i] = pow[i-1] * nn;

        seq_t num =
            (seq_t)865  * pow[9]  +   /* 173/63 */
            (seq_t)10815 * pow[8]  +   /* 103/3  */
            (seq_t)40950 * pow[7]  +   /* 130    */
            (seq_t)70560 * pow[6]  +   /* 224    */
            (seq_t)73206 * pow[5]  +   /* 1162/5 */
            (seq_t)80220 * pow[4]  +   /* 764/3  */
            (seq_t)89540 * pow[3]  +   /* 17908/63 */
            (seq_t)50400 * pow[2]  +   /* 160    */
            (seq_t)7749  * pow[1]  +   /* 123/5  */
            (seq_t)div;                /* constant term */

        seq_t result = num / (seq_t)div;
        return result;
    }
}
