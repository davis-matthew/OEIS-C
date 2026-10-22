/*
 * A359578  Dirichlet inverse of A336477, where A336477(n) = 1 if phi(n) is a
 *          power of 2, otherwise 0.
 * https://oeis.org/A359578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 9806, i16 n <= 9806, i32 n <= 9806, i64 n <= 9806,
 *            i128 n <= 9806, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the Dirichlet inverse of the characteristic function of numbers
 * whose totient is a power of two (A336477).  For n = Πp^e,
 *      a(1) = 1
 *      a(2^1) = -1,   a(2^e) = 0   for e > 1
 *      if p > 2 and p−1 is a power of two (i.e., p is a Fermat prime) then
 *          a(p^e) = (−1)^e
 *      otherwise a(p^e) = 0.
 * The function is implemented by prime factorisation. */

#include <stdint.h>
#include <stdlib.h>

/* Check whether v is a power of two.  v > 0 is assumed. */
static int is_power_of_two(uint64_t v) {
    return v && ((v & (v - 1)) == 0);
}

/* Given n ≥ 1, return a(n).  The function works for any order of calls. */
seq_t a(idx_t n) {
    if (n == 1)
        return 1;            /* a(1) = 1 */

    seq_t result = 1;
    idx_t remaining = n;

    /* Trial division: factorise n into prime powers. */
    for (idx_t p = 2; p <= remaining / p; ++p) {
        if (remaining % p != 0)
            continue;

        /* Count exponent of prime p. */
        idx_t exp = 0;
        while (remaining % p == 0) {
            remaining /= p;
            ++exp;
        }

        seq_t val;
        if (p == 2) {
            /* a(2^1) = -1, a(2^e) = 0 (e > 1). */
            val = (exp == 1) ? (seq_t)-1 : (seq_t)0;
        } else {
            /* For odd primes: check if p-1 is a power of two. */
            if (is_power_of_two(p - 1)) {
                /* a(p^e) = (−1)^e */
                val = (exp % 2 == 0) ? (seq_t)1 : (seq_t)-1;
            } else {
                val = 0;
            }
        }

        result *= val;
        if (result == 0)
            return 0;  /* Early exit if any factor contributes 0. */
    }

    /* Handle remaining factor > sqrt(original n), if any. */
    if (remaining > 1) {
        idx_t p = remaining;
        seq_t val;
        if (p == 2) {
            val = (seq_t)-1;          /* exponent is 1 */
        } else {
            if (is_power_of_two(p - 1))
                val = (seq_t)-1;      /* exponent is 1 */
            else
                val = 0;
        }

        result *= val;
        if (result == 0)
            return 0;
    }

    return result;
}
