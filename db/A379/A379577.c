/*
 * A379577  a(n) = (n!)^n + n^n.
 * https://oeis.org/A379577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n!)^n + n^n
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS sequence A379577 is defined as
 *
 *   a(n) = (n!)^n + n^n          for n ≥ 0
 *
 * with offset 0 (a(0)=2).  For n=0 we treat 0! = 1 and 0^0 = 1,
 * so the first term is 2.  For n>0 we use the supplied helper
 * functions:
 *
 *   - factorial(idx_t n)   → n!  (returns seq_t)
 *   - big_pow(seq_t base, seq_t exp) → base^exp  (returns seq_t)
 *
 * The function returns a(seq_t) with external linkage.
 *
 * The code follows the style constraints of the build system:
 *   * no static linkage or main() function
 *   * only the allowed arithmetic operators are used
 *   * no floating‑point or additional includes except the provided
 *     helper headers
 *   * extra safety for the n==0 case
 */

#include "../utils/factorial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the special case n == 0.  For n≥1 the helper functions
     * correctly compute factorial(0) = 1, but big_pow(0,0) is undefined,
     * so we treat it explicitly. */
    if (n == 0)
        return (seq_t)2;

    /* Compute n! as seq_t. */
    seq_t fac = factorial(n);

    /* Cast n to seq_t once for use as the exponent. */
    seq_t exp = (seq_t)n;

    /* Compute (n!)^n and n^n using big_pow. */
    seq_t term1 = big_pow(fac, exp);
    seq_t term2 = big_pow((seq_t)n, exp);

    /* Return the sum.  Arithmetic is performed in seq_t, which may
     * be an unsigned integer of different widths or an arbitrary‑
     * precision implementation. */
    return term1 + term2;
}
