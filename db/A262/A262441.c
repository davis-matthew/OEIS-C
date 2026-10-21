/*
 * A262441  a(n) = Sum_{k=0..n+1}(binomial(n-1,k)/(k+1)*binomial(n+k+1,n-k)).
 * https://oeis.org/A262441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 30, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A262441
    a(n) = Sum_{k=0..n+1} (binomial(n-1,k)/(k+1))*binomial(n+k+1,n-k)
    The following implementation uses the 3‑term recurrence from the OEIS entry:
    2*(n+1)*(2*n-1)*(19*n-30)*a(n) = 20*(19*n^3-49*n^2+34*n-6)*a(n-1)
                                   + 2*(n-2)*(38*n^2-79*n+15)*a(n-2)
                                   + 3*(n-3)*(n-2)*(19*n-11)*a(n-3)
    Initial values: a(0)=1, a(1)=2, a(2)=5
*/

#include <stdint.h>

#if defined(__cplusplus)
    /* Big‑integer implementation (C++). Use seq_t operators directly. */
    seq_t a(idx_t n) {
        if (n == 0UL) return (seq_t)1;
        if (n == 1UL) return (seq_t)2;
        if (n == 2UL) return (seq_t)5;

        seq_t a_nm3 = (seq_t)1;
        seq_t a_nm2 = (seq_t)2;
        seq_t a_nm1 = (seq_t)5;

        for (idx_t i = 3; i <= n; ++i) {
            seq_t tn = (seq_t)i;

            seq_t coeff_lhs = (seq_t)2 * (tn + (seq_t)1) *
                              ((seq_t)2 * tn - (seq_t)1) *
                              ((seq_t)19 * tn - (seq_t)30);

            seq_t coeff1 = (seq_t)20 * ((seq_t)19 * tn * tn * tn -
                                        (seq_t)49 * tn * tn +
                                        (seq_t)34 * tn - (seq_t)6);

            seq_t coeff2 = (seq_t)2 * ((tn - (seq_t)2) *
                                      ((seq_t)38 * tn * tn -
                                       (seq_t)79 * tn + (seq_t)15));

            seq_t coeff3 = (seq_t)3 * ((tn - (seq_t)3) *
                                      (tn - (seq_t)2) *
                                      ((seq_t)19 * tn - (seq_t)11));

            seq_t numerator = coeff1 * a_nm1 + coeff2 * a_nm2 + coeff3 * a_nm3;
            seq_t a_i = numerator / coeff_lhs;

            a_nm3 = a_nm2;
            a_nm2 = a_nm1;
            a_nm1 = a_i;
        }
        return a_nm1;
    }

#else
    /* 64‑bit integer implementation (C).  Use unsigned __int128 for intermediates. */
    static inline seq_t mul128(seq_t a, seq_t b) {
        return (seq_t)((__int128)a * b);
    }

    seq_t a(idx_t n) {
        if (n == 0UL) return (seq_t)1;
        if (n == 1UL) return (seq_t)2;
        if (n == 2UL) return (seq_t)5;

        seq_t a_nm3 = (seq_t)1;
        seq_t a_nm2 = (seq_t)2;
        seq_t a_nm1 = (seq_t)5;

        for (idx_t i = 3; i <= n; ++i) {
            __int128 tn = i;          /* use 128‑bit for intermediate values */

            __int128 coeff_lhs =
                (__int128)2 * (tn + 1) *
                (2 * tn - 1) *
                (19 * tn - 30);

            __int128 coeff1 =
                (__int128)20 * (19 * tn * tn * tn -
                                49 * tn * tn +
                                34 * tn - 6);

            __int128 coeff2 =
                (__int128)2 * ((tn - 2) *
                               (38 * tn * tn - 79 * tn + 15));

            __int128 coeff3 =
                (__int128)3 * ((tn - 3) *
                               (tn - 2) *
                               (19 * tn - 11));

            __int128 numerator =
                coeff1 * (__int128)a_nm1 +
                coeff2 * (__int128)a_nm2 +
                coeff3 * (__int128)a_nm3;

            seq_t a_i = (seq_t)(numerator / coeff_lhs);

            a_nm3 = a_nm2;
            a_nm2 = a_nm1;
            a_nm1 = a_i;
        }
        return a_nm1;
    }
#endif
