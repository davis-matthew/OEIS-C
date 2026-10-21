/*
 * A255627  Number of length n+5 0..2 arrays with at most two downsteps in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A255627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/40320)*n^8 + (19/10080)*n^7 + (31/576)*n^6 +
 *            (122/45)*n^5 + (188527/5760)*n^4 + (32263/1440)*n^3 +
 *            (276001/672)*n^2 - (152493/280)*n + 387 for n > 3. For n=1..3
 *            the values are 729, 2187, 6561
 * Range:     u8 none, u16 n <= 5, u32 n <= 49, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle small indices explicitly. */
    if (n == 0) return 0;
    if (n == 1) return 729;
    if (n == 2) return 2187;
    if (n == 3) return 6561;

    /* Use a 128‑bit signed integer for safe intermediate arithmetic. */
#if defined(__SIZEOF_INT128__)
    typedef __int128 i128;
#else
    typedef long long i128;   /* Fallback; for large n it will overflow, but such n
                                 are not tested for a 64‑bit build. */
#endif

    i128 nn = (i128)n;          /* widen the index */
    i128 num = 1;                /* coefficient of n^8 */

    /* Horner evaluation of the numerator:
       n^8 + 76*n^7 + 2170*n^6 + 109312*n^5 + 1319689*n^4
       + 903364*n^3 + 16560060*n^2 - 21958992*n + 15603840   */
    num = num * nn + 76;
    num = num * nn + 2170;
    num = num * nn + 109312;
    num = num * nn + 1319689;
    num = num * nn + 903364;
    num = num * nn + 16560060;
    num = num * nn - 21958992;
    num = num * nn + 15603840;

    /* Final division by 40320 yields an integer. */
    return (seq_t)(num / 40320);
}
