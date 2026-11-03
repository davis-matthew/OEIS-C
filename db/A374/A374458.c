/*
 * A374458  Squares of exponentially odd numbers (A268335).
 * https://oeis.org/A374458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (b(n))^2, where b(n) is the n-th positive integer whose *
 *            prime exponents are all odd. The function enumerates successive
 *            * integers, tests the exponent parity for each prime factor, and
 *            * accumulates the nth qualifying integer. Finally it returns its
 *            square
 * Range:     u8 n <= 12, u16 n <= 183, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9734 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Test whether all prime exponents in x are odd. */
static inline bool odd_exponent(seq_t x)
{
    seq_t t = x;
    seq_t p = 2;
    while (p * p <= t)
    {
        if (t % p == 0)
        {
            unsigned int exp = 0;
            while (t % p == 0)
            {
                t /= p;
                exp++;
            }
            if ((exp & 1) == 0)   /* even exponent found */
                return false;
        }
        ++p;
    }
    /* Remaining factor t > 1 has exponent 1 (odd). */
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 1; not used */

    idx_t count = 0;
    seq_t candidate = 1;
    while (count < n)
    {
        if (odd_exponent(candidate))
            ++count;
        ++candidate;
    }
    seq_t m = candidate - 1;   /* nth odd‑exponent number */
    return m * m;              /* its square */
}
