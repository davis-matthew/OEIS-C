/*
 * A251569  E.g.f.: exp(x*G(x)) where G(x) = 1 + x*G(x)^3 is the g.f. of
 *          A001764.
 * https://oeis.org/A251569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A251569: a(n) = exp(x*G(x)) where G(x)=1+x*G(x)^3
   Recurrence (n>=3):
      2*(2*n-1)*(54*n^2-171*n+116)*a(n) =
        (1458*n^4-7533*n^3+12474*n^2-6624*n-7)*a(n-1)
       -(324*n^3-1080*n^2+759*n+95)*a(n-2)
        + 8*(n-2)*(54*n^2-63*n-1)*a(n-3)
   Initial values: a(0)=1, a(1)=1, a(2)=3.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 0, 1, 2 */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)3;

    /* In order to avoid 64‑bit overflow we perform all intermediate
       arithmetic in a 128‑bit type on GCC/Clang.  If __int128 is not
       available we fall back to the same type as seq_t, which is
       sufficient for the arbitrary‑precision build. */
#if defined(__SIZEOF_INT128__)
    typedef unsigned __int128 big_t;
#else
    typedef seq_t big_t;
#endif

    big_t a0 = 1;   /* a(n-3) */
    big_t a1 = 1;   /* a(n-2) */
    big_t a2 = 3;   /* a(n-1) */

    for (idx_t i = (idx_t)3; i <= n; ++i) {
        big_t ii = (big_t)i;
        big_t i2 = ii * ii;
        big_t i3 = i2 * ii;
        big_t i4 = i2 * i2;

        /* --- Coefficient of a(n) on the left side --- */
        big_t C = 2 * (2 * ii - 1) *
                  (54 * i2 - 171 * ii + 116);

        /* --- Coefficients on the right side --- */
        big_t D1 = (1458 * i4) - (7533 * i3) +
                   (12474 * i2) - (6624 * ii) - 7;
        big_t D2 = (324 * i3) - (1080 * i2) +
                   (759 * ii) + 95;
        big_t D3 = 8 * (ii - 2) *
                  (54 * i2 - 63 * ii - 1);

        /* Compute RHS using big_t to stay exact */
        big_t rhs = D1 * a2 - D2 * a1 + D3 * a0;
        big_t nxt = rhs / C;           /* division exact */

        /* Roll the window */
        a0 = a1;
        a1 = a2;
        a2 = nxt;
    }

    return (seq_t)a2;   /* final result fits into seq_t */
}
