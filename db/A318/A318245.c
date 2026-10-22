/*
 * A318245  Scaled g.f. T(v) = Sum_{n>=0} a(n)*(3*v/64)^n satisfies
 *          9*(5*v-4)*T + d/dv(16*v*(v-1)*(3*v-4)*T') = 0, and a(0)=1;
 *          sequence gives a(n).
 * https://oeis.org/A318245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   3*n^2*a(n) = 4*(28*n^2-28*n+9)*a(n-1) -
 *            64*(4*n-5)*(4*n-3)*a(n-2), a(0)=1, a(1)=12
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
#ifdef __cplusplus
    // C++ compiler: choose implementation based on whether seq_t is an integral type
    if constexpr (std::is_integral<seq_t>::value) {
        // Integral implementation using 128‑bit intermediates
        if (n == 0) return (seq_t)1;
        if (n == 1) return (seq_t)12;
        seq_t a0 = 1;
        seq_t a1 = 12;
        for (idx_t i = 2; i <= n; ++i) {
            unsigned __int128 ii = i;
            unsigned __int128 coeff1 = 4 * ((unsigned __int128)28 * ii * ii
                                            - (unsigned __int128)28 * ii + 9);
            unsigned __int128 coeff2 = 64 * ((unsigned __int128)(4 * ii - 5))
                                            * ((unsigned __int128)(4 * ii - 3));
            unsigned __int128 denom = 3 * ii * ii;
            unsigned __int128 prod1 = (unsigned __int128)a1 * coeff1;
            unsigned __int128 prod2 = (unsigned __int128)a0 * coeff2;
            unsigned __int128 numer = prod1 - prod2;
            seq_t a2 = (seq_t)(numer / denom);
            a0 = a1;
            a1 = a2;
        }
        return a1;
    } else {
        // Arbitrary‐precision implementation (cases where seq_t is a big integer class)
        if (n == 0) return (seq_t)1;
        if (n == 1) return (seq_t)12;
        seq_t a0 = 1;
        seq_t a1 = 12;
        for (idx_t i = 2; i <= n; ++i) {
            seq_t ii = (seq_t)i;
            seq_t coeff1 = 4 * ((seq_t)28 * ii * ii - (seq_t)28 * ii + 9);
            seq_t coeff2 = 64 * ( (seq_t)(4 * ii - 5) ) *
                                    ( (seq_t)(4 * ii - 3) );
            seq_t denom = 3 * ii * ii;
            seq_t a2 = (coeff1 * a1 - coeff2 * a0) / denom;
            a0 = a1;
            a1 = a2;
        }
        return a1;
    }
#else
    // C compiler: integral implementation only, using 128‑bit intermediates
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)12;
    seq_t a0 = 1;
    seq_t a1 = 12;
    for (idx_t i = 2; i <= n; ++i) {
        unsigned __int128 ii = i;
        unsigned __int128 coeff1 = 4 * ((unsigned __int128)28 * ii * ii
                                        - (unsigned __int128)28 * ii + 9);
        unsigned __int128 coeff2 = 64 * ((unsigned __int128)(4 * ii - 5))
                                        * ((unsigned __int128)(4 * ii - 3));
        unsigned __int128 denom = 3 * ii * ii;
        unsigned __int128 prod1 = (unsigned __int128)a1 * coeff1;
        unsigned __int128 prod2 = (unsigned __int128)a0 * coeff2;
        unsigned __int128 numer = prod1 - prod2;
        seq_t a2 = (seq_t)(numer / denom);
        a0 = a1;
        a1 = a2;
    }
    return a1;
#endif
}
