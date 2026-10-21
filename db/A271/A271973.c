/*
 * A271973  Smallest number k such that gcd(s1, s2) = n, where s1 is the sum
 *          of the odd numbers and s2 is the sum of the even numbers in the
 *          Collatz (3x+1) trajectory of k.
 * https://oeis.org/A271973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the smallest positive integer k such that  
   the gcd of the sum of odd terms and the sum of even terms  
   in the Collatz trajectory of k equals the given number n. 
   The trajectory stops when it reaches 1. A^1 is counted as odd. 
   We compute this value on demand; no closed formula is known. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"

/* Computes gcd( sum of odd terms , sum of even terms ) for the Collatz
   trajectory of a starting value `start`. */
static seq_t collatz_gcd(seq_t start)
{
    seq_t s_odd = 0, s_even = 0;   /* running sums */
    seq_t cur   = start;

    while (1) {
        if (cur & 1U) {          /* odd term */
            s_odd += cur;
            if (cur == (seq_t)1) break;  /* reached the end */
            cur = cur * (seq_t)3 + (seq_t)1;
        } else {                 /* even term */
            s_even += cur;
            cur >>= 1;
        }
    }
    return gcd(s_odd, s_even);
}

/* Returns the nth term of A271973: the smallest k with gcd(...) == n. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* out of range, but avoid UB */

    seq_t target = (seq_t)n;
    for (idx_t k = 1; ; ++k) {
        if (collatz_gcd((seq_t)k) == target) return (seq_t)k;
    }
}
