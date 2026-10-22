/*
 * A366161  The number of ways to express n^n in the form a^b for integers a
 *          and b.
 * https://oeis.org/A366161
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   _gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of integer solutions (a,b) to a^b = n^n,
 *            counting negative a when b is even.
 *
 * The number of solutions equals
 *   a(n) = 2 * τ(n·g) - τ_odd(n·g)
 * where g = gcd of the exponents in the prime factorisation of n,
 * τ(m) is the number of positive divisors of m,
 * τ_odd(m) is the number of odd divisors of m
 * (i.e. τ(m) divided by the factor contributed by 2).
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_gcd_idx.h"

/* gcd for unsigned integers (idx_t) */

/* Count the number of ways to write n^n as a^b (integer a,b).  */
extern seq_t a(idx_t n) {
    if (n < 2) return 0;            /* undefined for n < 2, return 0 */

    /* ----- compute g = gcd of exponents of n ----- */
    idx_t x = n;
    idx_t g = 0;                    /* 0 means "no exponent yet" */
    for (idx_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            idx_t e = 0;
            do { x /= p; ++e; } while (x % p == 0);
            g = (g == 0) ? e : _gcd_idx(g, e);
        }
    }
    if (x > 1) {                    /* remaining prime factor with exponent 1 */
        g = (g == 0) ? 1 : _gcd_idx(g, 1);
    }

    /* ----- M = n * g ----- */
    seq_t M = (seq_t)n * (seq_t)g;
    seq_t M_tmp = M;

    /* ----- factor M to get τ(M) and exponent of 2 ----- */
    seq_t tau = 1;                  /* product of (exponent+1) */
    seq_t e2 = 0;                   /* exponent of prime 2 in M */

    for (seq_t p = 2; p * p <= M_tmp; ++p) {
        if (M_tmp % p == 0) {
            seq_t e = 0;
            while (M_tmp % p == 0) {
                M_tmp /= p;
                ++e;
            }
            tau *= (e + 1);
            if (p == 2) e2 = e;
        }
    }
    if (M_tmp > 1) {                /* last prime factor (may be 2) */
        tau *= 2ULL;
        if (M_tmp == 2) e2 = 1;
    }

    /* ----- compute result: a(n) = 2 * τ(M) - τ_odd(M) ----- */
    seq_t tau_odd = tau / (e2 + 1); /* divide out the (e2+1) factor */
    seq_t result = 2 * tau - tau_odd;
    return result;
}
