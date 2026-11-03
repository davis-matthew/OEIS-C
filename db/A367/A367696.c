/*
 * A367696  Numbers k such that k and k+1 are both exponentially odious
 *          numbers (A270428).
 * https://oeis.org/A367696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 196, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 8163 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the nth positive integer k for which both k and k+1
 * are exponentially odious numbers (i.e. all prime exponents in their
 * factorizations have an odd binary popcount).  This function searches
 * naturally for such k and returns the n-th one.  The implementation
 * factorises candidate integers using trial division.  It is correct
 * for any index n ≥ 1 and works in O(n·√k) time, easily under the
 * required limits for n ≤ 10 000.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return true if e has an odd number of 1-bits in its binary representation. */
static inline bool odd_popcount_uint64(uint64_t e)
{
    bool parity = 0;
    while (e) {
        parity ^= e & 1;
        e >>= 1;
    }
    return parity;
}

/* Return true if n is an exponentially odious number: each prime exponent
 * in n's factorization has an odd popcount.  1 is considered odious. */
static inline bool is_exponentially_odious_64(uint64_t n)
{
    if (n <= 1)            /* 1 has no prime factors → vacuously true */
        return true;

    uint64_t x = n;

    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            uint64_t e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            if (!odd_popcount_uint64(e))  /* even popcount → not odious */
                return false;
        }
    }

    /* Remaining prime factor has exponent 1 → popcount(1) == 1 (odd) */
    return true;
}

/* Sequence function as required by the problem description. */
seq_t a(idx_t n)
{
    idx_t count = 0;      /* number of terms found so far */
    uint64_t val   = 0;   /* candidate integer k       */

    while (count < n) {
        ++val;
        if (is_exponentially_odious_64(val) &&
            is_exponentially_odious_64(val + 1))
            ++count;
    }
    return (seq_t)val;   /* cast to the appropriate integer type */
}
