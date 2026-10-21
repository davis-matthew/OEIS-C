/*
 * A244650  G.f. A(x) satisfies: Sum_{k=0..n} [x^k] A(x)^n =
 *          binomial(5*n,2*n).
 * https://oeis.org/A244650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A244650:  n*(n+5)*a(n) = (n+1)*(7*n+20)*a(n-1) –
   (n+2)*(11*n+15)*a(n-2) + 5*(n+1)*(n+2)*a(n-3)
   a(0)=1, a(1)=9, a(2)=55
   Computes a(n) for any n >= 0 (any call order),
   using 128‑bit intermediates when seq_t is a small integer type. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the initial values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)55;

    /* Local storage for the last three values */
    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)9;   /* a(n-2) */
    seq_t a2 = (seq_t)55;  /* a(n-1) */
    seq_t an = 0;          /* a(n) will be computed */

    /* Work out whether we can use 128‑bit intermediates
       (safe when seq_t is a built‑in unsigned type of size <= 8). */
    if (sizeof(seq_t) <= 8) {
        /* All arithmetic is performed in __uint128_t to avoid overflow */
        for (idx_t i = 3; i <= n; ++i) {
            __uint128_t mul1   = (i+1) * (7*(unsigned long long)i + 20);
            __uint128_t term1  = mul1 * (unsigned long long)a2;

            __uint128_t mul2   = (i+2) * (11*(unsigned long long)i + 15);
            __uint128_t term2  = mul2 * (unsigned long long)a1;

            __uint128_t term3  = (__uint128_t)5 * (i+1) * (i+2) * (unsigned long long)a0;

            __uint128_t numer  = term1 - term2 + term3;
            __uint128_t denom  = (__uint128_t)i * (i+5);

            an = (seq_t)(numer / denom);   /* exact division */
            a0 = a1;
            a1 = a2;
            a2 = an;
        }
    } else {
        /* seq_t is already a big‑integer type, use its native operations */
        for (idx_t i = 3; i <= n; ++i) {
            seq_t term1 = a2 * (seq_t)(7*(unsigned long long)i + 20)
                                 * (seq_t)(i+1);
            seq_t term2 = a1 * (seq_t)(11*(unsigned long long)i + 15)
                                 * (seq_t)(i+2);
            seq_t term3 = a0 * (seq_t)5 * (seq_t)(i+1) * (seq_t)(i+2);

            seq_t numer = term1 - term2 + term3;
            seq_t denom = (seq_t)i * (i+5);
            an = numer / denom;
            a0 = a1;
            a1 = a2;
            a2 = an;
        }
    }
    return a2;
}
