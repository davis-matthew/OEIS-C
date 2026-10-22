/*
 * A304875  Least prime p2 > p1 such that n^2 = (p1 + p2)/2 and p1 is prime.
 * https://oeis.org/A304875
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p₂ > p₁ such that n² = (p₁ + p₂)/2
 * Range:     u8 n <= 7, u16 n <= 254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm searches p₂ from the smallest prime > n² upward.
   For each candidate p₂ we check whether S − p₂ (where S = 2·n²)
   is also prime.  The first such p₂ is the value of a(n). */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* lightweight primality test using trial division up to √n.
   Works for any seq_t that supports + − * / % operators. */
static inline bool is_prime(seq_t x)
{
    if (x < 2) return false;
    if (x == 2 || x == 3) return true;
    if ((x & 1) == 0 || x % 3 == 0) return false;

    seq_t limit = floor_sqrt(x);
    seq_t d = 5;
    while (d <= limit) {
        if (x % d == 0 || x % (d + 2) == 0) return false;
        d += 6;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;              /* undefined before the offset */

    seq_t n_sq   = (seq_t)n * (seq_t)n;      /* n²  */
    seq_t target = 2 * n_sq;                /* S = 2·n² */

    /* start searching for p₂ just above n² */
    seq_t p2 = n_sq + 1;
    if ((p2 & 1) == 0) p2++;                /* make it odd */

    /* p₂ must not exceed target‑2, otherwise p₁ < 2. */
    seq_t max_p2 = target - 2;
    for ( ; p2 <= max_p2; p2 += 2 ) {
        if (!is_prime(p2)) continue;        /* not a prime → skip */
        seq_t p1 = target - p2;
        if (p1 >= 2 && is_prime(p1))       /* both primes found */
            return p2;
    }
    return 0;                      /* should not happen for n ≥ 2 */
}
