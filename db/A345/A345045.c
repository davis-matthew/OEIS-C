/*
 * A345045  a(n) = A047994(n) / A345044(n), where A047994(n) is multiplicative
 *          with a(p^e) = p^e - 1, and A345044(n) gives the least common
 *          multiple of the same factors.
 * https://oeis.org/A345045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A047994(n) / A345044(n) (OEIS A345045)
 * Range:     u8 n <= 678, u16 n <= 9934, u32 n <= 9934, u64 n <= 9934,
 *            u128 n <= 9934, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A047994(n) is multiplicative with a(p^e) = p^e - 1.  The denominator
   is the least common multiple of the same values for the prime‑power
   factors of n.  For n = 1, a(1) = 1. */

#include <stdint.h>   /* for uint64_t that may be used as idx_t/seq_t */

/* Smallest prime factor of a number.  A simple trial division is used
   here so that no external helper is required and the function compiles
   with any seq_t/idx_t type. */
static uint64_t smallest_prime_factor(uint64_t n)
{
    if (n % 2 == 0) return 2;
    for (uint64_t p = 3; p * p <= n; p += 2)
        if (n % p == 0) return p;
    return n;          /* n is prime */
}

/* Euclidean GCD for seq_t values (seq_t supports % and /). */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;   /* a(1) = 1, and a(0) is undefined */

    uint64_t m = (uint64_t)n;      /* use uint64_t for factorisation loops */
    seq_t prod = (seq_t)1;         /* product of (p^e - 1) */
    seq_t lcm  = (seq_t)1;         /* lcm of the same factors */

    /* Factorise m by trial division */
    for (uint64_t p = 2; p * p <= m; ) {
        if (m % p != 0) {
            if (p == 2) p = 3;
            else p += 2;
            continue;
        }

        /* Count exponent e of prime p */
        uint64_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }

        /* Compute v = p^e as seq_t and factor = v - 1 */
        seq_t v = (seq_t)1;
        for (uint64_t i = 0; i < e; ++i) v *= (seq_t)p;
        seq_t factor = v - (seq_t)1;        /* p^e - 1 >= 1 */

        prod *= factor;

        seq_t g = gcd_seq_t(lcm, factor);
        lcm = (lcm / g) * factor;
    }

    /* If any prime factor > sqrt(n) remains (i.e., m > 1) */
    if (m > 1) {
        seq_t factor = ((seq_t)m) - (seq_t)1;
        prod *= factor;
        seq_t g = gcd_seq_t(lcm, factor);
        lcm = (lcm / g) * factor;
    }

    return prod / lcm;
}
