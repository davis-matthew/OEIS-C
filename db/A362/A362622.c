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
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-03)
 * Helpers:   gcd_u64, is_prime_u64, mulmod
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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd_u64.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"

/* Pollard–Rho: find a non‑trivial divisor of n (n composite) */
static uint64_t pollard(uint64_t n) {
    if (n % 2 == 0) return 2;
    uint64_t c = 1;
    while (1) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mulmod(x, x, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            uint64_t diff = x > y ? x - y : y - x;
            d = gcd_u64(diff, n);
        }
        if (d != n) return d;
        ++c;  /* try another constant */
    }
}

/* recursive smallest prime factor */
static uint64_t smallest_prime_factor(uint64_t n) {
    if (n % 2 == 0) return 2;
    if (is_prime_u64(n)) return n;
    uint64_t d = pollard(n);
    uint64_t s1 = smallest_prime_factor(d);
    uint64_t s2 = smallest_prime_factor(n / d);
    return (s1 < s2) ? s1 : s2;
}

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
