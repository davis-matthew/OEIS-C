/*
 * A344055  a(n) = 2^n * n! * [x^n](exp(2*x) * BesselI(1, x)).
 * https://oeis.org/A344055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A344055
   a(n) = 2^n * n! * coefficient of x^n in exp(2*x) * BesselI(1,x)
   Recurrence (n ≥ 2):
     a(0) = 0
     a(1) = 1
     a(n) = 4 * ( (2*n*n - n)*a(n-1) - 3*(n*n - n)*a(n-2) ) / (n*n - 1)
   (All arithmetic is performed in unsigned integers; a(n) is always positive.)
*/

#include <stdint.h>
#include <stdlib.h>

/* Define the function that returns a(n) for any non‑negative n. */
seq_t a(idx_t n)
{
    /* Handle the initial values directly. */
    if (n == 0) return 0;
    if (n == 1) return 1;

#if defined(__SIZEOF_INT128__)
    /* Use 128‑bit arithmetic for the intermediate computations
       (sufficient for uint8_t…uint64_t held in seq_t).    */
    __int128 a0 = 0;          /* a(0) */
    __int128 a1 = 1;          /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        __int128 N  = (__int128)i;      /* i as 128‑bit */
        __int128 NN = N * N;            /* i*i */

        /* Compute the two terms in the numerator.  All operations stay
           in 128‑bit to avoid overflow. */
        __int128 t1 = 3 * ((NN - N) * a0);          /* 3*(n^2‑n)*a(n‑2)   */
        __int128 t2 = (2 * NN - N) * a1;           /* (2*n^2‑n)*a(n‑1)   */

        /* Since t2 >= t1 for n ≥ 2, the difference is safe. */
        __int128 diff = t2 - t1;                   /* t2 – t1            */
        __int128 num  = 4 * diff;                  /* 4 * (t2 – t1)      */
        __int128 den  = NN - 1;                    /* n^2 – 1            */

        __int128 res = num / den;                  /* a(n)   */

        /* Shift for next iteration. */
        a0 = a1;
        a1 = res;
    }
    return (seq_t)a1;  /* cast back to seq_t; truncation is acceptable
                          for values that fit in seq_t. */

#else
    /* Fallback path (no __int128).  Works for arbitrary‑precision seq_t
       and for compilers that do provide 128‑bit arithmetic but do not
       expose __SIZEOF_INT128__.  All operations are performed with the
       seq_t type itself. */
    seq_t a0 = 0;        /* a(0) */
    seq_t a1 = 1;        /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t ii = (seq_t)i;          /* i as seq_t */
        seq_t NN = ii * ii;           /* i*i */

        seq_t t1 = 3 * ((NN - ii) * a0);          /* 3*(n^2‑n)*a(n‑2)   */
        seq_t t2 = (2 * NN - ii) * a1;           /* (2*n^2‑n)*a(n‑1)   */

        seq_t diff = t2 - t1;                   /* t2 – t1            */
        seq_t num  = 4 * diff;                  /* 4 * (t2 – t1)      */
        seq_t den  = NN - 1;                    /* n^2 – 1            */

        seq_t res = num / den;                  /* a(n)   */

        a0 = a1;
        a1 = res;
    }
    return a1;
#endif
}
