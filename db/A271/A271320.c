/*
 * A271320  Number of prime factors, with multiplicity, of the n-th n-gonal
 *          number (A060354).
 * https://oeis.org/A271320
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count_prime_factors( n*(n^2-3n+4)/2 )
 * Range:     u8 n <= 8, u16 n <= 41, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the number of prime factors (with multiplicity) of the
   n-th n-gonal number, i.e. the n-th element of A060354. */
#include <stdint.h>
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;              /* offset starts at 2; behaviour for <2 is irrelevant */

    seq_t ns   = (seq_t)n;
    seq_t ns2  = ns * ns;                 /* n^2  */
    seq_t part = ns2 - 3 * ns + 4;         /* n^2-3n+4, positive for n>=2 */
    seq_t num   = ns * part;              /* n*(n^2-3n+4) */
    seq_t pn     = num / 2;               /* the n-th n-gonal number */

    return count_prime_factors(pn);       /* number of prime factors with multiplicity */
}
