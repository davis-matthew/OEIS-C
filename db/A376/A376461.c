/*
 * A376461  a(n) = Sum_{k = 0..n} binomial(n, k)^2*binomial(n+k, k)*A108625(n,
 *          k).
 * https://oeis.org/A376461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A376461
   Recurrence: for n >= 3
      n^4*(171*n^2 - 551*n + 446) * a(n) =
          7*(2052*n^6 - 10716*n^5 + 21806*n^4 - 22301*n^3 + 12415*n^2 - 3630*n + 440) * a(n-1)
        + (27189*n^6 - 196365*n^5 + 577739*n^4 - 882118*n^3 + 733309*n^2 - 313390*n + 53944) * a(n-2)
        + (n-2)^4*(171*n^2 - 209*n + 66) * a(n-3)
   Initial values: a(0)=1, a(1)=7, a(2)=199
   The function works for any seq_t (built‑in unsigned integer up to 128 bits
   or an arbitrary‑precision class) and any idx_t (uint64_t). */

#include <stdint.h>

/* Choose a larger type for intermediate arithmetic.
   In C++ (arbitrary‑precision seq_t) we use seq_t itself.
   In C, we use unsigned __int128 if available, otherwise uint64_t. */
#if defined(__cplusplus)
typedef seq_t  big_t;
#define TO_BIG(x)  ((big_t)(x))
#else
#ifdef __SIZEOF_INT128__
typedef unsigned __int128 big_t;
#define TO_BIG(x) ((big_t)(x))
#else
typedef uint64_t big_t;
#define TO_BIG(x) ((big_t)(x))
#endif
#endif

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)199;

    seq_t a_nm3 = (seq_t)1;   /* a(0)   */
    seq_t a_nm2 = (seq_t)7;   /* a(1)   */
    seq_t a_nm1 = (seq_t)199; /* a(2)   */
    seq_t an    = (seq_t)0;   /* current term */

    for (idx_t i = 3; i <= n; ++i) {
        big_t i_big = TO_BIG(i);
        big_t i2 = i_big * i_big;          /* i^2   */
        big_t i3 = i2 * i_big;             /* i^3   */
        big_t i4 = i2 * i2;                /* i^4   */
        big_t i5 = i4 * i_big;             /* i^5   */
        big_t i6 = i4 * i2;                /* i^6   */

        big_t i_minus2 = i_big - (big_t)2;
        big_t ip2 = i_minus2 * i_minus2;            /* (i-2)^2 */
        big_t ip4 = ip2 * ip2;                     /* (i-2)^4 */

        /* LHS: n^4*(171*n^2 - 551*n + 446) */
        big_t L = i4 * ( (big_t)171 * i2 - (big_t)551 * i_big + (big_t)446 );

        /* RHS coefficients */
        big_t coeff1 = (big_t)7 * (
                          (big_t)2052 * i6
                        - (big_t)10716 * i5
                        + (big_t)21806 * i4
                        - (big_t)22301 * i3
                        + (big_t)12415 * i2
                        - (big_t)3630 * i_big
                        + (big_t)440
                      );
        big_t coeff2 = (big_t)27189 * i6
                       - (big_t)196365 * i5
                       + (big_t)577739 * i4
                       - (big_t)882118 * i3
                       + (big_t)733309 * i2
                       - (big_t)313390 * i_big
                       + (big_t)53944;

        big_t coeff3 = ip4 * (
                          (big_t)171 * i2
                        - (big_t)209 * i_big
                        + (big_t)66
                      );

        /* Compute RHS in the wider type */
        big_t term1 = coeff1 * TO_BIG(a_nm1);
        big_t term2 = coeff2 * TO_BIG(a_nm2);
        big_t term3 = coeff3 * TO_BIG(a_nm3);

        big_t RHS = term1 + term2 + term3;

        /* Exact division – the recurrence guarantees integrality */
        an = (seq_t)(RHS / L);

        /* Shift for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = an;
    }

    return an;
}
