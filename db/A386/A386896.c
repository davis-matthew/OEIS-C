/*
 * A386896  a(n) = Sum_{k=0..n} binomial(5*n+1,k) * binomial(3*n-k,n-k).
 * https://oeis.org/A386896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} binomial(5*n+1,k) * binomial(3*n-k,n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the known P‑recursive relation
   2*(n-1)*n*(2*n-3)*(2*n-1)*(41*n-52)*a(n)
   = 3*(n-1)*(2*n-3)*(5617*n^3-12741*n^2+8390*n-1512)*a(n-1)
     -20*(5*n-8)*(5*n-7)*(5*n-6)*(5*n-4)*(41*n-11)*a(n-2)   */
/* with a(0)=1, a(1)=9. */

#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  Helper: use 128‑bit arithmetic when seq_t is ≤ 8 bytes  */
#if defined(__SIZEOF_INT128__) || defined(__GNUC__) || defined(_MSC_VER)
/*  128‑bit type is available   */
static inline int use_int128(void) { return 1; }
#else
static inline int use_int128(void) { return 0; }
#endif
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* base values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;

    /* a(0) and a(1) */
    seq_t prev2 = (seq_t)1;   /* a(n-2) for n=2 initially */
    seq_t prev1 = (seq_t)9;   /* a(n-1) for n=2 initially */
    seq_t cur   = prev1;      /* will hold a(n) */

    for (idx_t i = 2; i <= n; ++i)
    {
        if (use_int128())
        {
            /* 128‑bit intermediate calculations for small seq_t */
            unsigned __int128 ii   = i;
            unsigned __int128 a1   = (unsigned __int128)prev1;
            unsigned __int128 a2   = (unsigned __int128)prev2;

            unsigned __int128 ii2 = ii * ii;
            unsigned __int128 ii3 = ii2 * ii;

            unsigned __int128 p1 =
                (unsigned __int128)5617 * ii3
                - (unsigned __int128)12741 * ii2
                + (unsigned __int128)8390 * ii
                - 1512;

            unsigned __int128 coeff1 =
                (unsigned __int128)3 * (ii - 1)
                * (unsigned __int128)(2 * ii - 3) * p1;

            unsigned __int128 coeff2 =
                (unsigned __int128)20
                * (unsigned __int128)(5 * ii - 8)
                * (unsigned __int128)(5 * ii - 7)
                * (unsigned __int128)(5 * ii - 6)
                * (unsigned __int128)(5 * ii - 4)
                * (unsigned __int128)(41 * ii - 11);

            unsigned __int128 denom =
                (unsigned __int128)2 * (ii - 1) * ii
                * (unsigned __int128)(2 * ii - 3)
                * (unsigned __int128)(2 * ii - 1)
                * (unsigned __int128)(41 * ii - 52);

            unsigned __int128 numerator =
                coeff1 * a1 - coeff2 * a2;

            cur = (seq_t)(numerator / denom);
        }
        else
        {
            /* Full seq_t arithmetic (arbitrary‑precision case) */
            seq_t ii   = (seq_t)i;
            seq_t ii2  = ii * ii;
            seq_t ii3  = ii2 * ii;

            seq_t p1 = (seq_t)5617 * ii3
                       - (seq_t)12741 * ii2
                       + (seq_t)8390 * ii
                       - 1512;

            seq_t coeff1 = (seq_t)3 * (ii - 1)
                           * (seq_t)(2 * ii - 3) * p1;

            seq_t coeff2 = (seq_t)20 * (seq_t)(5 * ii - 8)
                           * (seq_t)(5 * ii - 7)
                           * (seq_t)(5 * ii - 6)
                           * (seq_t)(5 * ii - 4)
                           * (seq_t)(41 * ii - 11);

            seq_t denom = (seq_t)2 * (ii - 1) * ii
                          * (seq_t)(2 * ii - 3)
                          * (seq_t)(2 * ii - 1)
                          * (seq_t)(41 * ii - 52);

            seq_t numerator = coeff1 * prev1 - coeff2 * prev2;
            cur = numerator / denom;
        }

        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
