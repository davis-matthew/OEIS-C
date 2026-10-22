/*
 * A318197  a(n) = 2^((n - 1)*(n + 2)/2)*3^(n*(n - 1))*n^n.
 * https://oeis.org/A318197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^((n - 1)*(n + 2)/2)*3^(n*(n - 1))*n^n
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 we return 0, but
       the offset side of OEIS is 1 so this value is never used. */
    if (n == 0) return 0;

    /* Convert the index to the result type, for use in n^n. */
    seq_t sn = (seq_t)n;

    /* Compute the exponents for powers of 2 and 3. */
    idx_t exp2 = (n - 1) * (n + 2) / 2;   /* (n-1)(n+2)/2 */
    idx_t exp3 = n * (n - 1);             /* n(n-1) */

    /* Evaluate the three factors.  The helper maʹkns use exponentiation
       by squaring, so they are efficient enough for the sizes we
       encounter in the test harness. */
    seq_t val2 = pow2(exp2);
    seq_t val3 = pow3(exp3);
    seq_t valn = big_pow(sn, sn);        /* n^n */

    /* Multiply the factors.  The result type may be 8,16,32,64,128
       bits or an arbitrary‑precision class; the multiplication uses
       the native operators. */
    return val2 * val3 * valn;
}
