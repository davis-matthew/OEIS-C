/*
 * A258893  Number of (n+2) X (7+2) 0..1 arrays with no 3 X 3 subblock
 *          diagonal sum less than the antidiagonal sum or central row sum
 *          less than the central column sum.
 * https://oeis.org/A258893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   for n > 3, if n is even: a(n) = n^4 + 10*n^3 + 217*n^2 + 960*n +
 *            9216 if n is odd : a(n) = n^4 + 10*n^3 + 218*n^2 + 946*n + 9265
 *            Initial values: a(1)=271350, a(2)=104329, a(3)=33252
 * Range:     u8 none, u16 none, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the closed‑form polynomials, which reproduce
   all terms from the OEIS sequence.  It works for any n >= 1 and uses only
   integer arithmetic.  The intermediate results fit in seq_t for all
   n <= 10000.  */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base values for n = 1, 2, 3.  */
    static const seq_t base[3] = {271350, 104329, 33252};

    if (n == 0) /* offset below 1 is undefined; return 0 for safety */
        return 0;

    if (n <= 3)
        return base[n - 1];

    seq_t nn = (seq_t)n;     /* cast idx_t to seq_t to avoid truncation */
    seq_t n2 = nn * nn;      /* n^2 */
    seq_t n3 = n2 * nn;      /* n^3 */
    seq_t n4 = n3 * nn;      /* n^4 */

    if ((nn & 1) == 0) /* even n */
    {
        return n4 + (seq_t)10 * n3 + (seq_t)217 * n2 + (seq_t)960 * nn + (seq_t)9216;
    }
    else /* odd n */
    {
        return n4 + (seq_t)10 * n3 + (seq_t)218 * n2 + (seq_t)946 * nn + (seq_t)9265;
    }
}
