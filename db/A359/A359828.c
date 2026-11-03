/*
 * A359828  Characteristic function for primitive elements of A235992.
 * https://oeis.org/A359828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 137 terms through n = 9000 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 1 if the arithmetic derivative of n is even, and there is
   no proper divisor d|n (1 < d < n) such that both d and n/d have even
   derivative.  Otherwise a(n) = 0.  */
#include <stdint.h>
#include <stdbool.h>

/* Returns true iff the arithmetic derivative of n is even. */
static inline bool derivative_even(idx_t n)
{
    if (n == 1) return true;               /* d(1)=0 */

    idx_t orig = n;
    idx_t temp = n;
    bool parity = false;                  /* parity of d(n), false=even */

    /* factor 2 */
    if ((temp & 1) == 0) {
        int e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;   /* divide by 2 */
            e++;
        }
        bool term = (e & 1) && ((orig >> 1) & 1);  /* (e odd) && (n/2 odd) */
        parity ^= term;
    }

    /* odd prime factors */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            bool term = (e & 1) && ((orig / p) & 1);
            parity ^= term;
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        /* e = 1 for this prime */
        bool term = ((orig / temp) & 1);   /* (1 odd) && (n/p odd) */
        parity ^= term;
    }

    return !parity;  /* true ⇔ parity == 0 (even) */
}

/* Integer square‑root (floor) for unsigned 64‑bit indices. */
static inline idx_t isqrt(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) <= n / (r + 1)) r++;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* OEIS offset starts at 1 */

    if (!derivative_even(n)) return 0;  /* first condition fails */

    if (n == 1) return 1;              /* 1 has no proper divisors */

    idx_t limit = isqrt(n);
    for (idx_t d = 2; d <= limit; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;
            if (derivative_even(d) && derivative_even(e))
                return 0;          /* found a forbidden pair */
        }
    }
    return 1;                         /* primitive under the rule */
}
