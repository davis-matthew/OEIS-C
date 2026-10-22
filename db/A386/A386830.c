/*
 * A386830  a(n) = Sum_{k=0..n} 3^k * 2^(n-k) * binomial(3*n+1,k) *
 *          binomial(3*n-k,n-k).
 * https://oeis.org/A386830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A386830
 *
 *  Formula: a(n) = [x^n] (1+3*x)^(3*n+1)/(1-2*x)^(2*n+1).
 *
 *  Recurrence:
 *      8*n*(2*n-3)*a(n) + 6*(-108*n^2+207*n-80)*a(n-1)
 *      + 405*(3*n-2)*(3*n-4)*a(n-2) = 0
 *
 *  Initial values:  a(0) = 1,  a(1) = 18.
 *
 *  The function returns the required term for any n ≥ 0.
 *  For small seq_t types the values after the first two terms may be
 *  truncated, which is allowed since those terms no longer fit into
 *  seq_t.  The routine keeps the intermediate arithmetic exact
 *  (using 128‑bit integers in C or the native arithmetic in C++).
 */

#include <stdint.h>

/*  Helper for 128‑bit multiplication on C builds.
 *  In C++ builds we rely on the native operators of seq_t. */
#if defined(__cplusplus)
  /* No special helper needed; plain multiplication is fine. */
#else
  static inline seq_t mul128(seq_t a, seq_t b)
  {
      return (seq_t)((unsigned __int128)a * b);
  }
#endif

seq_t a(idx_t n)
{
    /*  Base cases  */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)18;

    /*  Keep a(n-2) and a(n-1) to compute a(n)  */
    seq_t a_prev2 = (seq_t)1;      /* a(0)  */
    seq_t a_prev1 = (seq_t)18;     /* a(1)  */
    seq_t a_curr = 0;

    for (idx_t i = 2u; i <= n; ++i)
    {
#if defined(__cplusplus)
        /*  Fully arbitrary‑precision arithmetic  */
        seq_t coeff1 = (seq_t)648 * (seq_t)i * (seq_t)i
                       - (seq_t)1242 * (seq_t)i
                       + (seq_t)480;

        seq_t coeff2 = (seq_t)405 * ((seq_t)3*i - (seq_t)2)
                       * ((seq_t)3*i - (seq_t)4);

        seq_t denom = (seq_t)8*i * ((seq_t)2*i - (seq_t)3);

        seq_t term1 = coeff1 * a_prev1;
        seq_t term2 = coeff2 * a_prev2;
        seq_t numer = term1 - term2;
        a_curr = numer / denom;
#else
        /*  64‑bit indices – compute coefficients in idx_t  */
        idx_t coeff1_idx = 648*i*i - 1242*i + 480;
        idx_t coeff2_idx = 405*(3*i-2)*(3*i-4);
        idx_t denom_idx  = 8*i*(2*i-3);

        /*  cast to seq_t  */
        seq_t coeff1 = (seq_t)coeff1_idx;
        seq_t coeff2 = (seq_t)coeff2_idx;
        seq_t denom  = (seq_t)denom_idx;

        /*  128‑bit intermediate multiplication  */
        unsigned __int128 term1 = (unsigned __int128)coeff1 * a_prev1;
        unsigned __int128 term2 = (unsigned __int128)coeff2 * a_prev2;
        unsigned __int128 numer = term1 - term2;

        a_curr = (seq_t)( numer / (unsigned __int128)denom );
#endif
        /*  Shift for next iteration  */
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}
