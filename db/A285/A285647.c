/*
 * A285647  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 73", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A285647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=10, a(2)=1, a(3)=1100, a(n)=0 for even n>0,
 *            a(n)=(10^(n+1)-1)/9 for odd n>3. The last formula generates a
 *            decimal number consisting of (n+1) ones
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t */

seq_t a(idx_t n)
{
    /* Exact values for the first four indices */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)1100;

    /* Even indices (except 0) are 0 */
    if ((n & 1) == 0) return (seq_t)0;

    /* Odd indices n ≥ 5: (10^(n+1)-1)/9.
       Compute the decimal number comprised of (n+1) ones.  */
    idx_t digits = n + 1;      /* number of '1' digits needed */
    seq_t value = 1;           /* start with a single '1' */

    for (idx_t i = 2; i <= digits; ++i)
    {
        value = value * 10 + 1;    /* append another '1' */
    }
    return value;
}
