/*
 * A321642  Number of (7-n)-faces of the 4_21 polytope.
 * https://oeis.org/A321642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 240 * 2^{e2(n)} * 3^{e3(n)} * 7^{e7(n)} * where exponents
 *            are * n=0 : e2=0 , e3=4 , e7=0 * n=1 : e2=5 , e3=3 , e7=0 *
 *            n=2,3 : e2=5 , e3=2 , e7=1 * n=4 : e2=4 , e3=2 , e7=1 * n=5 :
 *            e2=2 , e3=2 , e7=1 * n=6 : e2=2 , e3=0 , e7=1 * n=7 : e2=0 ,
 *            e3=0 , e7=0 * For n>7 the polytope has no faces o...
 * Range:     u8 none, u16 n <= 0, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n > (idx_t)7)
        return 0;

    seq_t result = 240;      /* 240 = 2^4 * 3 * 5 */
    int e2 = 0, e3 = 0, e7 = 0;
    idx_t i = n;

    if (i == 0) {
        e3 = 4;            /* 3^4 */
    } else if (i == 1) {
        e2 = 5;            /* 2^5 */
        e3 = 3;            /* 3^3 */
    } else if (i >= 2 && i <= 5) {
        e7 = 1;            /* 7^1 */
        if (i <= 3) {      /* i = 2 or 3 */
            e2 = 5;        /* 2^5 */
            e3 = 2;        /* 3^2 */
        } else if (i == 4) {
            e2 = 4;        /* 2^4 */
            e3 = 2;        /* 3^2 */
        } else {          /* i == 5 */
            e2 = 2;        /* 2^2 */
            e3 = 2;        /* 3^2 */
        }
    } else if (i == 6) {
        e7 = 1;            /* 7^1 */
        e2 = 2;            /* 2^2 */
        /* e3 remains 0 */
    }                         /* i == 7: all exponents 0 */

    /* multiply the powers; safe for any seq_t */
    while (e2-- > 0) result *= (seq_t)2;
    while (e3-- > 0) result *= (seq_t)3;
    while (e7-- > 0) result *= (seq_t)7;

    return result;
}
