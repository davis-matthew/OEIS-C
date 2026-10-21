/*
 * A256387  Numbers n such that no prime can be the arithmetic mean of 2
 *          semiprimes whose difference is 2*n.
 * https://oeis.org/A256387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th odd integer n such that not(is_prime(n+4) &&
 *            is_semiprime(2*n+4))
 * Range:     u8 n <= 111, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* n is 1‑based and a(1) = 5.  The rule guarantees that no prime can be the
 * arithmetic mean of two semiprimes whose difference is 2*n. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime.h"
#include "../utils/_smallest_prime_factor.h"
#include "../utils/seq_realloc.h"

/* test whether an integer m is a semiprime (product of two primes, with multiplicity) */
static bool is_semiprime(uint64_t m)
{
    if (m < 4) return false;
    uint64_t sp = _smallest_prime_factor(m);
    if (sp == 0) return false;        /* safety for pathological input      */
    if (sp == m) return false;        /* m is prime – not semiprime          */
    uint64_t q = m / sp;
    return is_prime((idx_t)q);
}

/* test whether an integer n satisfies the sequence condition */
static bool is_allowed(uint64_t n)
{
    if ((n & 1U) == 0) return false;  /* only odd n can belong to the sequence */
    if (!is_prime((idx_t)(n + 4))) return true;
    if (!is_semiprime(2 * n + 4))   return true;
    return false;
}

/* return the n-th member of the sequence (1‑based) */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t count = 0;
    static uint64_t candidate = 5;      /* start from the first odd > 1 */

    /* generate terms until we have at least n of them */
    while (count < n) {
        /* advance candidate to the next odd number satisfying the rule */
        while (true) {
            if (is_allowed(candidate)) {
                /* grow the array one slot */
                terms = seq_realloc(terms, count, count + 1);
                terms[count] = (seq_t)candidate;
                ++count;
                candidate += 2;   /* next odd candidate */
                break;
            }
            candidate += 2;       /* skip this odd failed candidate */
        }
    }
    return terms[n - 1];
}
