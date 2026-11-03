/*
 * A285800  Numbers having more than one odd prime factor to an odd power in
 *          their prime factorization.
 * https://oeis.org/A285800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th integer having at least two distinct odd prime
 *            factors that occur to an odd exponent in its prime factorization
 * Range:     u8 n <= 85, u16 n <= 19999, u32 n <= 19999, u64 n <= 19999,
 *            u128 n <= 19999, big any n
 * Validated: standard, pass, 72 terms through n = 9468 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: return true if x has at least two distinct odd prime factors
   whose exponents are odd. */
static bool has_two_odd_prime_odds(seq_t x) {
    idx_t t = (idx_t)x;          /* work with native integer for factorisation */

    /* Remove all factors of 2 – the prime 2 is not counted */
    while (t % 2 == 0) {
        t /= 2;
    }

    int count = 0;             /* number of odd primes with odd exponent */

    idx_t p = 3;
    while (p * p <= t) {
        int exp = 0;
        while (t % p == 0) {
            t /= p;
            exp++;
        }
        if ((exp & 1) == 1) { /* exponent odd? */
            count++;
            if (count >= 2) return true;   /* early exit */
        }
        p += 2;                 /* next odd candidate */
    }

    /* Remaining t > 1 is an odd prime with exponent 1 (odd) */
    if (t > 1) {
        count++;
    }

    return count >= 2;
}

seq_t a(idx_t n) {
    idx_t found   = 0;    /* number of terms found so far */
    idx_t candidate = 1;  /* current integer to test */

    while (found < n) {
        candidate++;               /* test next integer */
        if (has_two_odd_prime_odds(candidate)) {
            found++;               /* one more term found */
        }
    }

    return (seq_t)candidate;       /* the n-th term */
}
