/*
 * A333312  Positive integers m >= 2 such that there are at least two values
 *          of v such that the sequence {v, ..., v + m - 1} of m nonnegative
 *          integers can be partitioned into two subsets of consecutive
 *          integers with the same sum.
 * https://oeis.org/A333312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A333312 :  m ≥ 2 such that there are at least two
 *  values of v for which the consecutive block
 *  {v,…,v+m‑1} can be split into two consecutive subsets
 *  with equal sum.
 *
 *  This is equivalent to:
 *  • m odd composite  (i.e. odd and not prime), or
 *  • m > 12, m divisible by 4, and m not a power of two.
 *
 *  The algorithm simply scans integers m ≥ 2, applies the
 *  above test, and counts qualifying numbers until the
 *  nth one is found.
 *
 *  Complexity :  O(n log n) for typical primality tests,
 *  fast enough for n up to about 10 000.          */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Index type : unsigned 64‑bit */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 2;

    while (true)
    {
        bool good = false;

        if ((m & 1ULL) != 0)              /* odd */
        {
            /* exclude 1 and odd primes */
            if (m > 2 && !is_prime(m))
                good = true;
        }
        else if ((m % 4ULL) == 0ULL)      /* divisible by 4 */
        {
            /* must be >12 and not a power of two */
            if (m > 12ULL && ((m & (m - 1ULL)) != 0ULL))
                good = true;
        }
        /* even numbers ≡ 2 (mod 4) are automatically excluded */

        if (good)
        {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
        ++m;
    }
}
