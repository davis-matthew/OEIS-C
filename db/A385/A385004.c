/*
 * A385004  a(n) = Sum_{k=0..n} 2^(n-k) * binomial(3*n,k).
 * https://oeis.org/A385004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} 2^(n‑k) * binomial(3*n,k) (recurrence form)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* Arbitrary‑precision (C++17) implementation */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    seq_t a_nm2 = (seq_t)1; /* a(0) */
    seq_t a_nm1 = (seq_t)5; /* a(1) */
    seq_t a_n   = a_nm1;    /* placeholder */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t nseq = (seq_t)i;

        seq_t LHS  = 8 * nseq * (2 * nseq - 1) * (15 * nseq - 23);
        seq_t RHS1 = 6 * ( (540 * nseq * nseq * nseq)
                           - (1503 * nseq * nseq)
                           + (1239 * nseq)
                           - 320 );

        seq_t RHS2 = 81 * ( (3 * nseq - 5)
                            * (3 * nseq - 4)
                            * (15 * nseq - 8) );

        seq_t tmp  = RHS1 * a_nm1 - RHS2 * a_nm2;
        a_n        = tmp / LHS;

        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;

#else
    /* C11 implementation – seq_t is an unsigned integer type (uint8_t…uint64_t) */
#if defined(__SIZEOF_INT128__) && __SIZEOF_INT128__ > 0
    typedef unsigned __int128 uint128_t;

    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    seq_t a_nm2 = (seq_t)1; /* a(0) */
    seq_t a_nm1 = (seq_t)5; /* a(1) */
    seq_t a_n   = a_nm1;    /* placeholder */

    for (idx_t i = 2; i <= n; ++i)
    {
        uint128_t n128 = (uint128_t)i;

        uint128_t LHS  = 8 * n128 * (2 * n128 - 1) * (15 * n128 - 23);
        uint128_t RHS1 = 6 * ( (540 * n128 * n128 * n128)
                               - (1503 * n128 * n128)
                               + (1239 * n128)
                               - 320 );

        uint128_t RHS2 = 81 * ( (3 * n128 - 5)
                                * (3 * n128 - 4)
                                * (15 * n128 - 8) );

        uint128_t tmp  = RHS1 * (uint128_t)a_nm1 - RHS2 * (uint128_t)a_nm2;
        a_n            = (seq_t)(tmp / LHS);

        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
#else
    /* NO_IMPLEMENTATION: __int128 not available; cannot safely compute the sequence. */
    return (seq_t)0;
#endif
#endif
}
