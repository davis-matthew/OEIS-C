/*
 * A387369  a(n) = Sum_{k=0..n} 2^k * 3^(n-k) * binomial(n+2,k) *
 *          binomial(n+2,n-k).
 * https://oeis.org/A387369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^k * 3^(n-k) * binomial(n+2,k) *
 *            binomial(n+2,n-k) Recurrence: a(0)=1, a(1)=15; n*(n+4)*a(n) =
 *            (n+2)*(5*(2*n+3)*a(n-1) - (n+1)*a(n-2)) for n>1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Implement the recurrence directly.  For C++ (arbitrary‑precision) we
   can rely on the built‑in operators of the big integer class.  For
   C (integral seq_t) we use __uint128_t intermediates to avoid overflow. */
#ifdef __cplusplus
/* C++ implementation – safe for big‑integer types. */
seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)15;

    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)15;  /* a(1) */
    seq_t a_n   = (seq_t)0;

    for (idx_t i = 2u; i <= n; ++i)
    {
        seq_t coeff1 = (seq_t)5 * (seq_t)(2 * i + 3);   /* 5*(2*i+3) */
        seq_t coeff2 = (seq_t)(i + 1);                  /* i+1      */

        seq_t term1 = coeff1 * a_nm1;  /* 5*(2*i+3)*a(n-1) */
        seq_t term2 = coeff2 * a_nm2;  /* (i+1)*a(n-2)     */
        seq_t diff  = term1 - term2;

        seq_t num = diff * (seq_t)(i + 2);                  /* (i+2)*diff  */
        seq_t den = (seq_t)(i) * (seq_t)(i + 4);            /* i*(i+4)     */

        a_n = num / den;                                   /* exact division */

        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
#else
/* C implementation – use __uint128_t for intermediates. */
seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)15;

    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)15;  /* a(1) */
    seq_t a_n   = (seq_t)0;

    for (idx_t i = 2u; i <= n; ++i)
    {
        uint64_t coeff1 = 5ULL * (2ULL * i + 3ULL);      /* 5*(2*i+3) */
        uint64_t coeff2 = i + 1ULL;                     /* i+1      */

        __uint128_t term1 = (__uint128_t)coeff1 * a_nm1; /* 5*(2*i+3)*a(n-1) */
        __uint128_t term2 = (__uint128_t)coeff2 * a_nm2; /* (i+1)*a(n-2)     */
        __uint128_t diff  = term1 - term2;

        __uint128_t num = diff * (i + 2ULL);             /* (i+2)*diff  */
        __uint128_t den = (__uint128_t)i * (i + 4ULL);   /* i*(i+4)     */

        a_n = (seq_t)(num / den);                       /* cast after division */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
#endif
