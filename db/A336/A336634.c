/*
 * A336634  Sum_{n>=0} a(n) * x^n / (n!)^2 = exp(-x) * BesselI(0,2*sqrt(x))^2.
 * https://oeis.org/A336634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   Recurrence * n*a(n) = -(3*n^2 - 7*n + 3)*a(n - 1) * + (7 -
 *            3*n)*(n - 1)^2*a(n - 2) * - (n - 1)^2*(n - 2)^2*a(n - 3) * with
 *            a(0)=1, a(1)=1, a(2)=0
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 13, i64 n <= 21, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++: choose the implementation that fits the seq_t size. */
    if constexpr (sizeof(seq_t) <= 8) {
        /* Builtin integer build – use 128‑bit intermediate. */
        if (n == 0) return (seq_t)1;
        if (n == 1) return (seq_t)1;
        if (n == 2) return (seq_t)0;
        seq_t a_nm3 = (seq_t)1; /* a(0) */
        seq_t a_nm2 = (seq_t)1; /* a(1) */
        seq_t a_nm1 = (seq_t)0; /* a(2) */
        seq_t an   = (seq_t)0;
        for (idx_t k = 3; k <= n; ++k) {
            __int128 km1 = k - 1;
            __int128 km2 = k - 2;
            __int128 coeff1 = -((__int128)3 * k * k - (__int128)7 * k + (__int128)3);
            __int128 coeff2 = ((__int128)7 - (__int128)3 * k);
            __int128 term1 = coeff1 * (__int128)a_nm1;
            __int128 term2 = coeff2 * km1 * km1 * (__int128)a_nm2;
            __int128 term3 = -km1 * km1 * km2 * km2 * (__int128)a_nm3;
            __int128 numer = term1 + term2 + term3;
            an = (seq_t)(numer / k);
            a_nm3 = a_nm2;
            a_nm2 = a_nm1;
            a_nm1 = an;
        }
        return an;
    } else {
        /* Arbitrary‑precision build – use seq_t for all arithmetic. */
        if (n == 0) return (seq_t)1;
        if (n == 1) return (seq_t)1;
        if (n == 2) return (seq_t)0;
        seq_t a_nm3 = (seq_t)1; /* a(0) */
        seq_t a_nm2 = (seq_t)1; /* a(1) */
        seq_t a_nm1 = (seq_t)0; /* a(2) */
        seq_t an   = (seq_t)0;
        for (idx_t k = 3; k <= n; ++k) {
            seq_t k_t  = (seq_t)k;
            seq_t km1  = k_t - 1;
            seq_t km2  = k_t - 2;
            seq_t k2   = k_t * k_t;
            seq_t coeff1 = -((seq_t)3 * k2 - (seq_t)7 * k_t + (seq_t)3);
            seq_t coeff2 = ((seq_t)7 - (seq_t)3 * k_t);
            seq_t term1 = coeff1 * a_nm1;
            seq_t term2 = coeff2 * km1 * km1 * a_nm2;
            seq_t term3 = -(km1 * km1 * km2 * km2 * a_nm3);
            seq_t numer = term1 + term2 + term3;
            an = numer / k_t;
            a_nm3 = a_nm2;
            a_nm2 = a_nm1;
            a_nm1 = an;
        }
        return an;
    }
#else
    /* C: assume seq_t is a builtin integer type – use 128‑bit intermediate. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;
    seq_t a_nm3 = (seq_t)1; /* a(0) */
    seq_t a_nm2 = (seq_t)1; /* a(1) */
    seq_t a_nm1 = (seq_t)0; /* a(2) */
    seq_t an   = (seq_t)0;
    for (idx_t k = 3; k <= n; ++k) {
        __int128 km1 = k - 1;
        __int128 km2 = k - 2;
        __int128 coeff1 = -((__int128)3 * k * k - (__int128)7 * k + (__int128)3);
        __int128 coeff2 = ((__int128)7 - (__int128)3 * k);
        __int128 term1 = coeff1 * (__int128)a_nm1;
        __int128 term2 = coeff2 * km1 * km1 * (__int128)a_nm2;
        __int128 term3 = -km1 * km1 * km2 * km2 * (__int128)a_nm3;
        __int128 numer = term1 + term2 + term3;
        an = (seq_t)(numer / k);
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = an;
    }
    return an;
#endif
}
