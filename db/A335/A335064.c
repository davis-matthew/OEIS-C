/*
 * A335064  Let m = d*q + r be the Euclidean division of m by d. The terms m
 *          of this sequence satisfy that q, r, d are consecutive positive
 *          integer terms in a geometric progression with a noninteger common
 *          ratio > 1.
 * https://oeis.org/A335064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   divisor_greater_than_s, factor_unique_primes, in_A024619
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A335064:   a(n) = A024619(n) * (A024619(n)+1)
 *
 *  A024619 consists of all positive integers k for which 
 *  there exist integers p, s, q' with
 *          p > s ≥ 2 ,  gcd(p,s)=1,
 *          k = p * s * q'.
 *
 *  A brute‑force test for k is implemented: 
 *  For each divisor s (2 ≤ s < √k) that divides k with s² < k,
 *    let t = k / s.  
 *    If t has any divisor p > s that is coprime with s,
 *    then k ∈ A024619.
 *
 *  The divisor test is performed by factorising t and taking the
 *  product of those prime powers that are not contained in the
 *  prime factorisation of s.  If that maximal product exceeds s,
 *  the desired divisor exists.
 *
 *  The function enumerates k in increasing order until the n‑th
 *  member of A024619 is found and returns its oblong number.
 *
 *  Complexity:   O(N √K) with very small constants; fast for
 *  n ≤ 10 000.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/divisor_greater_than_s.h"
#include "../utils/factor_unique_primes.h"
#include "../utils/in_A024619.h"

/* Main routine requested by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* unused for offset 1 */
    uint64_t count = 0;
    uint64_t k = 1;

    while (true) {
        ++k;
        if (in_A024619(k)) {
            ++count;
            if (count == n) {
                /* return k*(k+1) in the required seq_t type   */
                return (seq_t)k * (seq_t)(k + 1);
            }
        }
    }
}
