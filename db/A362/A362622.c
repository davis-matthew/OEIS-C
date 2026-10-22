/*
 * A362622  One and numbers whose prime factorization has its greatest part at
 *          a middle position.
 * https://oeis.org/A362622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n‑th positive integer whose prime factorisation
   has its greatest part at a middle position of the multiset of prime
   factors (counted with multiplicity).  The value 1 is included as a
   valid term.  The function searches the positive integers in order
   and returns the n‑th such number. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/smallest_prime_factor.h"

/* Return true if v satisfies the condition described in the comment
   above.  For v == 1 the condition is considered true. */
static bool satisfies(uint64_t v)
{
    if (v == 1) return true;

    uint64_t n = v;
    uint64_t largest = 0;   /* largest prime factor seen so far */
    size_t   expLargest = 0;/* multiplicity of the largest prime factor */
    size_t   k = 0;         /* total multiplicity of all prime factors */

    while (n > 1U) {
        uint64_t p = smallest_prime_factor(n);
        size_t   e = 0;
        while (n % p == 0U) {
            n /= p;
            ++e;
            ++k;
        }
        if (p > largest) {
            largest = p;
            expLargest = e;
        } else if (p == largest) {
            expLargest += e;
        }
    }

    /* Positions of the largest prime factor (1‑based) are from
       k - expLargest + 1 to k.  Midpoints are:
       - if k is odd: (k+1)/2
       - if k is even: k/2 and k/2+1
    */
    size_t firstLargestPos = k - expLargest + 1; /* 1‑based */

    if (k & 1U) { /* odd total multiplicity */
        size_t mid = k / 2U + 1U;
        return mid >= firstLargestPos;
    } else {      /* even total multiplicity */
        size_t mid1 = k / 2U;
        size_t mid2 = mid1 + 1U;
        return (mid1 >= firstLargestPos) || (mid2 >= firstLargestPos);
    }
}

/* a(idx_t n):  return the nth term of the sequence (offset = 1). */
seq_t a(idx_t n)
{
    uint64_t val = 1ULL;
    idx_t    cnt = 0U;

    while (true) {
        if (satisfies(val)) {
            ++cnt;
            if (cnt == n) {
                return (seq_t)val;
            }
        }
        ++val;
    }
}
