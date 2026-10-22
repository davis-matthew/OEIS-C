/*
 * A327407  Number of steps to reach a fixed point starting with n and
 *          repeatedly taking the quotient over the maximum divisor that is 1,
 *          prime, or whose prime indices are pairwise coprime. (A327389,
 *          A327401).
 * https://oeis.org/A327407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   gcd_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = number of steps to reach 1 starting with n and repeatedly
   dividing by the maximum divisor d of the current value such that
   d is 1, prime, or its prime indices (with multiplicity) are pairwise
   coprime.  Implemented by factorizing the current value, enumerating
   all admissible divisor combinations, and iterating until the value
   equals 1.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd_int.h"

/* Euclidean gcd for int arguments */

/* Check primality of a uint64_t (fast for small numbers) */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

/* Return the position (1‑based) of prime p in the sequence of primes */
static int prime_index(uint64_t p)
{
    int idx = 0;
    for (uint64_t n = 2; n <= p; ++n)
        if (is_prime_uint64(n))
            ++idx;
    return idx;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)   /* a(1) = 0 */
        return (seq_t)0;

    seq_t steps = 0;
    uint64_t cur = (uint64_t)n;

    while (cur > 1) {
        /* Factorization of cur */
        uint64_t primes[64];
        int       exps[64];
        int       count = 0;

        uint64_t temp = cur;
        int e2 = 0;                  /* exponent of 2 */
        while ((temp & 1ULL) == 0) { /* divide out 2 */
            temp >>= 1;
            ++e2;
        }
        if (e2 > 0) {
            primes[count] = 2;
            exps[count]   = e2;
            ++count;
        }

        for (uint64_t p = 3; p * p <= temp; p += 2) {
            if (temp % p == 0) {
                int e = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++e;
                }
                primes[count] = p;
                exps[count]   = e;
                ++count;
            }
        }
        if (temp > 1) { /* remaining prime factor */
            primes[count] = temp;
            exps[count]   = 1;
            ++count;
        }

        /* Separate prime 2 */
        int max_e2 = 0;
        for (int i = 0; i < count; ++i)
            if (primes[i] == 2) {
                max_e2 = exps[i];
                break;
            }

        /* Collect other primes and their indices */
        uint64_t others[64];
        int       other_idx[64];
        int       other_cnt = 0;
        for (int i = 0; i < count; ++i) {
            if (primes[i] == 2) continue;
            others[other_cnt]   = primes[i];
            other_idx[other_cnt] = prime_index(primes[i]);
            ++other_cnt;
        }

        /* Enumerate all valid divisor combinations */
        uint64_t best = 1; /* 1 is always allowed */
        int total_masks = 1 << other_cnt;
        for (int mask = 0; mask < total_masks; ++mask) {
            bool ok = true;
            uint64_t prod = 1;

            /* Check pairwise coprime prime indices */
            for (int i = 0; i < other_cnt; ++i) {
                if (!(mask & (1 << i))) continue;
                int idx_i = other_idx[i];
                for (int j = 0; j < i; ++j) {
                    if (!(mask & (1 << j))) continue;
                    if (gcd_int(idx_i, other_idx[j]) != 1) {
                        ok = false;
                        break;
                    }
                }
                if (!ok) break;
                prod *= others[i];
            }
            if (!ok) continue;

            /* Add powers of 2 up to max_e2 */
            for (int e = 0; e <= max_e2; ++e) {
                uint64_t d = prod << e; /* multiply by 2^e */
                if (d > best) best = d;
            }
        }

        /* Divide cur by the maximal admissible divisor */
        cur /= best;
        ++steps;
    }

    return (seq_t)steps;
}
