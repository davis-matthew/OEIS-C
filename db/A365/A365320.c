/*
 * A365320  Number of pairs of distinct positive integers <= n that cannot be
 *          linearly combined with nonnegative coefficients to obtain n.
 * https://oeis.org/A365320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered pairs {p,q} with * 1 ≤ p < q ≤ n for
 *            which there are no non‑negative integers * x,y such that p*x +
 *            q*y = n
 * Range:     u8 n <= 28, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   modinv_coprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/modinv_coprime.h"

/* Greatest common divisor of two unsigned integers */
static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Return true iff non‑negative integer solutions exist for
 * p*x + q*y = n.  The argument n is assumed to be a multiple
 * of gcd(p,q). */
static inline bool pair_represents(idx_t p, idx_t q, idx_t n) {
    if (p == 1 || q == 1) return true; /* x = n, y = 0 works */

    idx_t g = gcd_idx(p, q);
    if (n % g != 0) return false;      /* gcd must divide n */

    idx_t p1 = p / g, q1 = q / g;
    idx_t m  = n / g;
    if (p1 == 1 || q1 == 1) return true; /* reduced pair contains 1 */

    /* Find a particular solution (x0, y0) with 0 ≤ x0 < q1 */
    idx_t inv = modinv_coprime(p1, q1);
    idx_t x0 = ((idx_t)(m % q1) * inv) % q1;

    /* Compute corresponding y0 ; may be negative */
    long long y0 = (long long)m - (long long)p1 * (long long)x0;
    y0 /= (long long)q1;

    return y0 >= 0;  /* if y0 >= 0, we already have a solution;
                       otherwise no non‑negative solution exists */
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* For n < 5 the sequence is zero as per the offset */
    if (n < 5) return (seq_t)0;

    seq_t result = 0;
    for (idx_t p = 1; p < n; ++p) {
        for (idx_t q = p + 1; q <= n; ++q) {
            if (!pair_represents(p, q, n)) ++result;
        }
    }
    return result;
}
