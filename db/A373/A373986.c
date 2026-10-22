/*
 * A373986  Numerator of A373158(n) / A108951(n), where A108951 is fully
 *          multiplicative and A373158 is fully additive with a(p) = p# for
 *          prime p, where x# is the primorial A034386(x).
 * https://oeis.org/A373986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A373158(n) / gcd(A108951(n), A373158(n))
 * Range:     u8 n <= 20, u16 n <= 33, u32 n <= 57, u64 n <= 117,
 *            u128 n <= 205, big any n
 * Helpers:   _smallest_prime_factor, build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          A373158(n) = Σ e · p#  (prime power p^e ∥ n)   */
/*          A108951(n) = Π (p#)^e  (prime power p^e ∥ n)   */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/gcd.h"
#include "../utils/_smallest_prime_factor.h"
#include "../utils/build_primes_upto.h"

/* Global data: list of primes up to the maximum prime needed so far. */
static uint64_t *prime_list   = NULL;   /* array of primes, sorted ascending */
static size_t   prime_cnt    = 0;        /* number of primes currently stored */

/* Ensure that the prime list contains all primes ≤ limit.  If it
   already contains at least limit, nothing is done.  A new list is
   built with build_primes_upto, the old list is freed. */
static void ensure_primes_upto(uint64_t limit)
{
    if (prime_cnt != 0 && prime_list[prime_cnt - 1] >= limit)
        return;                     /* already sufficient */

    uint64_t *new_primes;
    size_t    new_cnt;
    build_primes_upto(limit, &new_primes, &new_cnt);  /* allocates with malloc */

    free(prime_list);              /* free old list (or NULL, no-op) */
    prime_list = new_primes;
    prime_cnt  = new_cnt;
}

/* Return the primorial (product of all primes ≤ p) for a given prime p.
   The list of primes is assumed to contain p (ensure_primes_upto(p) must
   have been called before).  The primorial is calculated by walking
   through the prime list and multiplying until p is reached.  Since
   seq_t may be an arbitrary‑precision class, we use the type directly
   without any risk of UB. */
static seq_t primorial_of_prime(uint64_t p)
{
    seq_t prod = 1;
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        prod *= (seq_t)prime_list[i];
        if (prime_list[i] == p)
            return prod;
    }
    /* Should never happen – rebuild if needed */
    ensure_primes_upto(p);
    prod = 1;
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        prod *= (seq_t)prime_list[i];
        if (prime_list[i] == p)
            return prod;
    }
    return prod;  /* fallback; in practice unreachable */
}

/* The requested OEIS sequence. */
seq_t a(idx_t n)
{
    /* OEIS offset is 1.  For n==1 the definition yields 0. */
    if (n == 1)
        return 0;

    uint64_t temp = (uint64_t)n;
    seq_t    sum  = 0;   /* A373158(n)   */
    seq_t    prod = 1;   /* A108951(n)   */

    while (temp != 1)
    {
        uint64_t prime = _smallest_prime_factor(temp);
        int      exp   = 0;
        while (temp % prime == 0)
        {
            temp /= prime;
            ++exp;
        }

        ensure_primes_upto(prime);
        seq_t prim = primorial_of_prime(prime);

        sum += (seq_t)exp * prim;

        /* Multiply prod by prim^exp.  For p^e, A108951 multiplies (p#)^e. */
        for (int i = 0; i < exp; ++i)
            prod *= prim;
    }

    seq_t g = gcd(prod, sum);
    return sum / g;
}
