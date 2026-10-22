/*
 * A306821  Inverse binomial transform of the "original" Bernoulli numbers
 *          [A164555(n)/A027642(n)] with 1 and 1/2 swapped. Denominators.
 * https://oeis.org/A306821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A306821:  a(0)=2, a(1)=2.
   For n≥2:
     - if n is odd:     a(n)=1
     - if n is even:    a(n)= ( ∏_{p prime, (p‑1)|n} p ) / 2
   The product runs over all primes p≤n+1 such that n mod (p‑1)=0.
   This equals the denominator of the even‑index Bernoulli number Bₙ
   divided by 2 (and 1 for odd indices >1).
*/

#include <stdint.h>
#include <stdlib.h>

/* static storage for primes */
static uint32_t *prime_list = NULL;     /* array of primes */
static size_t prime_count = 0;          /* number of primes */
static uint64_t sieve_limit = 0;        /* maximum n where primes are known */

/* enlarge the prime table up to 'limit' (inclusive) */
static void ensure_primes(uint64_t limit)
{
    if (limit <= sieve_limit) return;

    /* temporary sieve array */
    uint8_t *is_prime = (uint8_t*)malloc((size_t)(limit + 1));
    if (!is_prime) return;   /* allocation failed; silently abort */

    for (uint64_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    if (limit >= 0) {
        is_prime[0] = 0;
        if (limit >= 1) is_prime[1] = 0;
    }

    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= limit; j += i) is_prime[j] = 0;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i) if (is_prime[i]) ++cnt;

    /* allocate new prime list */
    uint32_t *new_list = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!new_list) {
        free(is_prime);
        return;
    }

    size_t pos = 0;
    for (uint64_t i = 2; i <= limit; ++i) if (is_prime[i]) new_list[pos++] = (uint32_t)i;

    free(prime_list);          /* discard old list, if any */
    prime_list = new_list;
    prime_count = cnt;
    sieve_limit = limit;

    free(is_prime);
}

/* sequence element */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)2;      /* a(1)=2 */

    /* odd indices (except 1) are 1 */
    if (n & 1) return (seq_t)1;

    /* even indices ≥2 */
    uint64_t u_n = (uint64_t)n;
    ensure_primes(u_n + 1);           /* need primes up to n+1 */

    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p > u_n + 1) break;
        uint64_t d = (uint64_t)p - 1;
        if (d && (u_n % d == 0)) {
            prod *= (seq_t)p;
        }
    }

    prod /= (seq_t)2;                 /* denominator of Bₙ divided by 2 */
    return prod;
}
