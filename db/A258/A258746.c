/*
 * A258746  Permutation of the positive integers: this permutation transforms
 *          the enumeration system of positive irreducible fractions
 *          A007305/A047679 (Stern-Brocot) into the enumeration system
 *          A162909/A162910 (Bird), and vice versa.
 * https://oeis.org/A258746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 9367, u32 n <= 9367, u64 n <= 9367,
 *            u128 n <= 9367, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A258746: Permutation of the positive integers
   Defined by:  a(1)=1, a(2)=2, a(3)=3.
   For n>=2, let p = ⌊n/2⌋ and m = ⌊log₂(p)⌋.
   If m is even:  a(2p)   = 2·a(p);    a(2p+1) = 2·a(p)+1.
   If m is odd :  a(2p)   = 2·a(p)+1;  a(2p+1) = 2·a(p).
   The implementation uses recursion with the parent index p.
*/

#include <stdint.h>

/* Recursive helper to compute the sequence value. */
static seq_t a_rec(idx_t n)
{
    if (n == 1)                 /* base case */
        return 1;

    /* Parent index */
    idx_t p = n >> 1;           /* floor(n/2) */
    seq_t ap = a_rec(p);        /* a(p) */

    /* Compute m = floor(log₂(p)) */
    idx_t m = 0;
    idx_t tmp = p;
    while ((tmp >>= 1) != 0)
        ++m;

    /* Compute a(n) from ap according to the parity of m and n */
    seq_t val2 = ap * 2;        /* 2*a(p) */
    seq_t result;

    if ((m & 1) == 0) {           /* m even */
        if ((n & 1) == 0)         /* n even: a(2p) = 2*a(p) */
            result = val2;
        else                      /* n odd:  a(2p+1) = 2*a(p)+1 */
            result = val2 + 1;
    } else {                       /* m odd */
        if ((n & 1) == 0)         /* n even: a(2p) = 2*a(p)+1 */
            result = val2 + 1;
        else                      /* n odd:  a(2p+1) = 2*a(p) */
            result = val2;
    }

    return result;
}

/* External function implementing the sequence */
seq_t a(idx_t n)
{
    return a_rec(n);
}
