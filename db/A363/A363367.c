/*
 * A363367  a(n) is the least integer i >= 0 such that (i + 1) * (i + 2*n) / 2
 *          = p^2, p prime number (A000040), or a(n) = -1 if no such i exists.
 * https://oeis.org/A363367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 64, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the least i ≥ 0 such that (i+1)*(i+2*n)/2 = p^2
   with p a prime, or –1 if no such i exists
/*

   The search interval is 0 ≤ i ≤ 2*(n–1); the smallest i is returned.
   Zero is returned for n=0 or n=1 when no solution exists.
*/

#include <stdbool.h>
#include <stdint.h>

/* ---------- static helper: integer square root (floor) ----------
   Works for positive values of any seq_t size (up to 128‑bit)        */
static seq_t
isqrt_seq(seq_t x)
{
    /* Use binary search in unsigned __int128 to avoid overflow          */
    unsigned __int128 lo = 0;
    unsigned __int128 hi = (unsigned __int128)(~(uint64_t)0);
    /* Adjust hi to a reasonable upper bound: sqrt(x)+1                  */
    // For large x, hi may wrap; set hi to 1 << ((bitlen_seq_t(x)+1)/2)
    // But bitlen_seq_t may not be available; instead, use hi = 1 << 65
    // while ((unsigned __int128)hi*hi <= x) hi <<= 1;  // exponential
    // We'll use a simpler approach: hi = 1;
    if (x == 0) return 0;
    hi = 1;
    while ((unsigned __int128)hi * hi <= x) hi <<= 1;
    if (hi > ((unsigned __int128)~(uint64_t)0)) hi = ~(uint64_t)0; // clamp

    while (lo <= hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq == x) return (seq_t)mid;
        if (sq < x) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return (seq_t)(lo - 1);
}

/* ---------- static helper: primality test for small uint64_t -------
   Deterministic trial division up to sqrt(n)                           */
static bool
is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    uint64_t i = 5;
    uint64_t w = 2;
    uint64_t lim = (uint64_t)isqrt_seq((seq_t)n);
    while (i <= lim) {
        if (n % i == 0) return false;
        i += w;
        w = 6 - w;   // alternate increments of 2 and 4
    }
    return true;
}

/* ---------- main sequence function --------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)-1;

    /* maximum i: 2*(n-1) */
    idx_t i_max = 2 * n - 2;
    for (idx_t i = 0; i <= i_max; ++i) {
        /* compute product (i+1)*(i+2*n) in seq_t */
        seq_t a_term = (seq_t)(i + 1);
        seq_t b_term = (seq_t)(i + 2 * n);
        seq_t prod   = a_term * b_term;

        /* prod must be even so we can divide by 2 cleanly */
        if (prod & 1) continue;
        seq_t y = prod >> 1;          /* y = p^2 */

        /* compute p = sqrt(y) */
        seq_t p = isqrt_seq(y);
        if (p * p != y) continue;     /* not a perfect square */

        /* check primality of p */
        if (is_prime_uint64((uint64_t)p))
            return (seq_t)i;           /* minimal i found */
    }
    return (seq_t)-1;                   /* no solution in range */
}
