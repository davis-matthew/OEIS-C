/*
 * A389450  Sum of unitary divisors of the largest unitary divisor of n that
 *          is a term in A048103 (not divisible by p^p for any prime p).
 * https://oeis.org/A389450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(p^e) = p^e + 1 if e < p, 1 otherwise. The sequence is
 *            multiplicative, so a(n) = Π a(p^e) over the prime factorisation
 *            of n. Input n is an idx_t >= 1. This implementation performs a
 *            simple trial‑division factorisation and uses only integer
 *            arithmetic
 * Range:     u8 n <= 137, u16 n <= 9975, u32 n <= 9975, u64 n <= 9975,
 *            u128 n <= 9975, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Calculate a(n).  Uses only standard C operators and the custom
   integer type seq_t supplied by the build. */
seq_t a(idx_t n)
{
    /* For n==0 the behaviour is undefined – the sequence is only
       defined for n>=1. But we still handle it gracefully. */
    if (n == (idx_t)0) return (seq_t)0;

    uint64_t nn = (uint64_t)n;   /* cast to native integer for factorisation */
    seq_t result = (seq_t)1;

    /* ---------- factorise nn ---------- */
    uint64_t t = nn;
    /* handle factor 2 */
    uint64_t exp = 0;
    while ((t & 1ULL) == 0ULL) {   /* t % 2 == 0 */
        t >>= 1;
        exp++;
    }
    if (exp > 0) {
        /* compute 2^exp in seq_t */
        seq_t p_pow = (seq_t)1;
        for (uint64_t i = 0; i < exp; ++i) p_pow *= (seq_t)2;
        /* add factor contribution */
        if (exp < 2) { /* exp < p (p=2) */
            result *= (p_pow + (seq_t)1);
        }
    }

    /* handle odd factors */
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p != 0) continue;
        exp = 0;
        while (t % p == 0) {
            t /= p;
            exp++;
        }
        /* compute p^exp in seq_t */
        seq_t p_pow = (seq_t)1;
        for (uint64_t i = 0; i < exp; ++i)
            p_pow *= (seq_t)p;

        if (exp < p) {
            result *= (p_pow + (seq_t)1);
        }
    }

    /* If a prime factor remains (t > 1) it occurs to the first power. */
    if (t > 1) {
        seq_t p_pow = (seq_t)t;   /* p^1 */
        /* exp is 1, always < p for any prime p>1 */
        result *= (p_pow + (seq_t)1);
    }

    return result;
}
