/*
 * A253563  Permutation of natural numbers: a(0) = 1, a(1) = 2; after which,
 *          a(2n) = A253560(a(n)), a(2n+1) = A253550(a(n)).
 * https://oeis.org/A253563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   a(0) = 1, a(1) = 2; for n >= 1: a(2n) = A253560(a(n)), a(2n+1) =
 *            A253550(a(n)). A253560(x) = x * L(x) where L(x) is the largest
 *            prime factor of x. A253550(x) = x * N(L(x)) / L(x) where N(p) is
 *            the next prime after p
 * Range:     u8 n <= 47, u16 n <= 447, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   floor_sqrt, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"
#include "../utils/next_prime.h"

/* Return the largest prime factor of x.  For x <= 1 the result is x itself. */
static seq_t largest_prime_factor(seq_t x) {
    seq_t result = (seq_t)1;
    if (x <= 1) return x;

    /* factor 2 */
    while ((x % (seq_t)2) == 0) {
        result = (seq_t)2;
        x /= (seq_t)2;
    }

    /* factor odd primes */
    uint64_t p = 3;
    seq_t sqrtx = floor_sqrt(x);
    while ((seq_t)p <= sqrtx) {
        if ((x % (seq_t)p) == 0) {
            result = (seq_t)p;
            do {
                x /= (seq_t)p;
                sqrtx = floor_sqrt(x);
            } while ((x % (seq_t)p) == 0);
        }
        p = next_prime(p);
    }

    /* the remaining x is a prime > 1 */
    if (x > (seq_t)1) result = x;
    return result;
}

/* A253560(x) = x * L(x)  */
static seq_t A253560(seq_t x) {
    seq_t lpf = largest_prime_factor(x);
    return x * lpf;
}

/* A253550(x) = x * N(L(x)) / L(x)  */
static seq_t A253550(seq_t x) {
    seq_t lpf = largest_prime_factor(x);
    uint64_t next = next_prime((uint64_t)lpf);
    /* X and next may be large but operations are exact. */
    return (x * (seq_t)next) / lpf;
}

/* Main sequence function.  Uses recursion; no caching needed. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    idx_t parent = n >> 1; /* floor(n/2) */
    seq_t parent_val = a(parent);

    if ((n & 1) == 0)        /* even n -> left child */
        return A253560(parent_val);
    else                     /* odd n -> right child */
        return A253550(parent_val);
}
