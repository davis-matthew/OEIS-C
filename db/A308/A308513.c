/*
 * A308513  Sum of the largest side lengths of all integer-sided triangles
 *          with perimeter n whose smallest side length is prime.
 * https://oeis.org/A308513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p prime ≤ n/3} sum_{b = max(p,
 *            floor((n‑2p)/2)+1)}^{floor((n‑p)/2)} (n‑p‑b)
 * Range:     u8 n <= 40, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of the largest side lengths of all integer‑sided
   triangles with perimeter n whose smallest side is prime.
   The algorithm counts, for each prime smallest side p,
   all admissible second sides b with the largest side automatically
   determined as c = n – p – b.  The inner sum is evaluated
   analytically to obtain an O(π(n)) implementation.  All
   arithmetic is performed in seq_t, and the divisibility test for
   prime numbers is performed with a simple trial division,
   which is fast enough for the intended n (≈10000). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Simple primality test for small numbers (p ≤ n/3 ≤ Nmax/3). */
static inline bool is_prime_small(idx_t n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;
    for (idx_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n < 6) return 0;                /* smallest perimeter with a prime smallest side is 6 (2+2+2) */

    seq_t total = 0;
    idx_t a_max = n / 3;                /* largest possible smallest side */

    for (idx_t a = 2; a <= a_max; ++a) {
        if (!is_prime_small(a)) continue;

        /* lower bound for b ensuring a + b > c */
        idx_t lb1 = a;                                    /* b ≥ a  (sorting) */
        idx_t lb2 = ((n > 2 * a) ? ((n - 2 * a) / 2) : 0) + 1; /* floor((n-2a)/2)+1 */
        idx_t b_min = (lb1 > lb2) ? lb1 : lb2;

        /* upper bound for b ensuring b ≤ c */
        idx_t b_max = (n - a) / 2;

        if (b_min > b_max) continue;   /* no valid b for this a */

        idx_t count = b_max - b_min + 1;   /* number of admissible b’s */

        /* sum of b over the interval [b_min, b_max] */
        seq_t sum_b = ((seq_t)(b_min + b_max) * (seq_t)count) / 2;

        /* sum of c = n - a - b for all admissible b */
        seq_t sum_c = (seq_t)count * (seq_t)(n - a) - sum_b;

        total += sum_c;
    }
    return total;
}
