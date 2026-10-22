/*
 * A377059  a(n) is the smallest even r less than n-1 such that x^r = 1 (mod
 *          n) for the least x such that gcd(x,n)=1 for n >= 4 else 0.
 * https://oeis.org/A377059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 342, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A377059: a(n) is the smallest even multiplicative order
   of the least integer x > 1 with gcd(x,n)=1 (or 0 for n<4). */

#include <stdint.h>

static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;

    idx_t limit = n - 1;          /* r must be < n‑1 */

    for (idx_t x = 2; x < n; ++x) {
        if (gcd_idx(x, n) != 1)
            continue;

        idx_t p = x % n;
        idx_t r = 1;

        /* Compute multiplicative order of x modulo n. */
        while (p != 1) {
            p = (p * x) % n;
            ++r;
            if (r >= limit)
                break;  /* safety guard, should not happen */
        }

        if (p == 1 && (r % 2) == 0 && r < limit)
            return (seq_t)r;
    }

    return 0;
}
