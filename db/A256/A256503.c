/*
 * A256503  Smallest k>=1 such that n^2 + (n+1)^2 + ... + (n+k)^2 is prime or
 *          a(n)=0 if there is no such k.
 * https://oeis.org/A256503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k ≥ 1 such that Σ_{i=0}^{k} (n+i)^2 is prime;
 *            computed via closed forms: - if 2n^2+2n+1 is prime → k = 1 -
 *            else if 3n^2+6n+5 is prime → k = 2 - else if 6n^2+30n+55 is
 *            prime → k = 5 - otherwise a(n) = 0 (Offset is 1: a(1) is the
 *            first term.)
 * Range:     u8 n <= 7, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, but catch 0 safely */

    seq_t nn = (seq_t)n;

    seq_t p1 = 2 * nn * nn + 2 * nn + 1;
    if (isPrimeGeneric(p1)) return (seq_t)1;

    seq_t p2 = 3 * nn * nn + 6 * nn + 5;
    if (isPrimeGeneric(p2)) return (seq_t)2;

    seq_t p3 = 6 * nn * nn + 30 * nn + 55;
    if (isPrimeGeneric(p3)) return (seq_t)5;

    return (seq_t)0;
}
