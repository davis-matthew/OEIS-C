/*
 * A349712  a(n) = Sum_{d|n} sopf(d) * sopf(n/d).
 * https://oeis.org/A349712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 9623, u32 n <= 9623, u64 n <= 9623,
 *            u128 n <= 9623, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Formula: a(n) = Sum_{d|n} sopf(d) * sopf(n/d)
   where sopf(m) is the sum of distinct prime factors of m. */

/* The function is deliberately simple and incurs only a small
   amount of work for each call.  It does not rely on any globals
   beyond a list of primes, which is built lazily the first time it
   is needed. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* idx_t and seq_t are supplied by the build system.  We only use
   them for indexing and as the return/value type.  No 'static'
   is used for seq_t because it might be an arbitrary-precision
   class; we only allocate arrays of native integer types.  */

static uint64_t *prime_list   = NULL;   /* list of primes (uint64_t) */
static size_t   prime_count  = 0;       /* number of primes stored   */
static size_t   prime_cap    = 0;       /* allocated capacity       */
static uint64_t prime_limit  = 0;       /* largest number for which
                                          primes are guaranteed to exist */

/* Expand the prime list up to at least `limit`.  The function
   uses trial division by all already known primes.  */
static void ensure_primes_up_to(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    idx_t start = (prime_limit > 1 ? prime_limit + 1 : 2);
    if (limit < start)
        return;

    for (idx_t i = start; i <= limit; ++i) {
        bool prime = true;
        if (i % 2 == 0 && i != 2)
            prime = false;
        else {
            for (size_t pi = 0; pi < prime_count; ++pi) {
                uint64_t p = prime_list[pi];
                if (p * p > i)
                    break;
                if (i % p == 0) {
                    prime = false;
                    break;
                }
            }
        }
        if (prime) {
            if (prime_count == prime_cap) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 8;
                uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
                if (!tmp)
                    abort();
                prime_list = tmp;
                prime_cap  = new_cap;
            }
            prime_list[prime_count++] = (uint64_t)i;
        }
    }
    prime_limit = limit;
}

/* Sum of distinct prime factors of `n`.  */
static seq_t sopf(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    seq_t sum = 0;
    idx_t temp = n;
    idx_t limit = (idx_t)sqrt((long double)temp);
    ensure_primes_up_to(limit);

    for (size_t pi = 0; pi < prime_count; ++pi) {
        uint64_t p = prime_list[pi];
        if (p > limit)
            break;
        if (temp % p == 0) {
            sum += (seq_t)p;
            while (temp % p == 0)
                temp /= p;
            limit = (idx_t)sqrt((long double)temp);
        }
    }
    if (temp > 1)          /* `temp` is now a prime > sqrt(original n) */
        sum += (seq_t)temp;

    return sum;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* undefined for n < 1, return 0 */

    seq_t result = 0;
    /* Iterate over divisors up to sqrt(n) and count both the divisor
       and its complementary divisor, since each contributes separately. */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d2 = n / d;
            seq_t s1 = sopf(d);
            seq_t s2 = sopf(d2);
            result += s1 * s2;      /* divisor d */
            if (d != d2)            /* complementary divisor d2 */
                result += s2 * s1;  /* same product, different divisor */
        }
    }
    return result;
}
