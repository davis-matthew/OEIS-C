/*
 * A353511  Positions of odd terms in A001001, where A001001(n) = Sum_{d|n}
 *          d*sigma(d).
 * https://oeis.org/A353511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 213, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9195 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A353511
 *  Positions of odd terms in A001001(n) = Σ_{d|n} d * σ(d)
 *  Criterion: n yields an odd A001001(n) iff every odd prime exponent in n
 *  is congruent to 0 or 1 modulo 4.  The function a(n) returns the nth
 *  integer with this property (1‑based index). */
 
#include <stdint.h>
#include <stdbool.h>

/* Test whether n satisfies the odd‑term condition described above. */
static bool is_odd_term(idx_t n) {
    /* Factors of 2 do not affect the parity condition. */
    while ((n & 1u) == 0)
        n >>= 1;
    /* Test exponents of remaining odd primes. */
    for (idx_t p = 3; ; p += 2) {
        if (p > n / p)        /* p*p > n */
            break;
        if (n % p != 0)
            continue;
        /* Count multiplicity of prime p. */
        idx_t exp = 0;
        while (n % p == 0) {
            n /= p;
            exp++;
        }
        /* exp must be 0 or 1 modulo 4. */
        if ((exp & 3u) > 1u)   /* exp % 4 == 2 or 3 */
            return false;
    }
    /* If n > 1, it is an odd prime factor with exponent 1, which is allowed. */
    return true;
}

/* Main sequence function.  Linear search suffices for n up to ≈10000. */
seq_t a(idx_t n) {
    idx_t count = 0;           /* how many qualifying numbers seen */
    idx_t cur   = 1;           /* current integer to test */

    while (1) {
        if (is_odd_term(cur)) {
            count++;
            if (count == n)
                return (seq_t)cur;   /* conversion works for both integer and big types */
        }
        cur++;
    }
}
