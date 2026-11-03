/*
 * A373149  Fully additive with a(2) = 1 and a(p) = prevprime(p) for odd
 *          primes p.
 * https://oeis.org/A373149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{p^e‖n} e * a(p), a(2)=1, a(p)=prev_prime(p) for odd
 *            primes p
 * Range:     u8 n <= 262, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 107 terms through n = 9853 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A373149 – fully additive over prime factorisation */
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return the largest prime < n   (n > 2)   ; pp(2) = 1, but we never call it for n==2 */
static inline idx_t prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p)
    {
        if (is_prime(p)) return p;
    }
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;                  /* a(1) = 0 */

    seq_t result = (seq_t)0;
    idx_t temp   = n;

    /* factor 2 */
    if (temp & 1) {                 /* temp is odd, skip 2 */
    } else {
        idx_t e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++e;
        }
        result += (seq_t)e * (seq_t)1; /* a(2) = 1 */
    }

    /* factor odd primes */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            do {
                temp /= p;
                ++e;
            } while (temp % p == 0);
            seq_t ap = (seq_t)prev_prime(p); /* a(p) = prev_prime(p) for odd p */
            result += (seq_t)e * ap;
        }
    }

    /* if remaining temp > 1, it is a prime factor */
    if (temp > 1) {
        seq_t ap = (temp == 2) ? (seq_t)1 : (seq_t)prev_prime(temp);
        result += ap;                      /* exponent is 1 */
    }

    return result;
}
