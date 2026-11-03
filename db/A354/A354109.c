/*
 * A354109  Numbers that are neither an odd prime power nor twice an odd prime
 *          power.
 * https://oeis.org/A354109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 75 terms through n = 75 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A354109: numbers that are neither an odd prime power nor
   twice an odd prime power. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Test whether an ordinary unsigned 64‑bit integer is an odd prime power */
static bool is_odd_prime_power_u64(uint64_t n)
{
    if (n < 3 || (n & 1) == 0)
        return false;                 /* less than 3 or even → false */

    uint64_t p;
    for (p = 3; p * p <= n; p += 2)   /* trial divide by odd numbers */
        if (n % p == 0)
            break;                    /* found a divisor */

    if (p * p > n)                    /* no divisor → n is prime (odd) */
        return true;

    /* n divisible by p – check if it is a pure power of p */
    uint64_t tmp = n;
    while (tmp % p == 0)
        tmp /= p;
    return tmp == 1;                  /* true iff n == p^k */
}

/* Determine whether a seq_t value is excluded (odd prime power or twice) */
static bool is_excluded(seq_t m)
{
    /* The values that arise here for n <= 10000 comfortably fit in 64 bits */
    uint64_t n = (uint64_t)m;

    if (is_odd_prime_power_u64(n))
        return true;

    if ((n & 1) == 0) {               /* even numbers: test half */
        uint64_t half = n >> 1;
        if (is_odd_prime_power_u64(half))
            return true;
    }
    return false;
}

/* Main sequence accessor: returns the nth term (1‑based index) */
seq_t a(idx_t n)
{
    seq_t candidate = (seq_t)1;    /* start from 1 */
    idx_t   count    = 0;          /* number of valid terms seen */

    while (1) {
        if (!is_excluded(candidate)) {         /* keep the term if not excluded */
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;                                   /* try next integer */
    }
}
