/*
 * A295433  a(n) = (12*n)!*n!/((8*n)!*(3*n)!*(2*n)!).
 * https://oeis.org/A295433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n)! * n! / ((8*n)! * (3*n)! * (2*n)!)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*=== Prime sieve cache ====================================================*/
/* Keeps a list of primes up to the current limit across calls.            */
static void ensure_primes(idx_t limit, idx_t **primes_out, size_t *cnt_out)
{
    static idx_t *primes = NULL;              /* list of primes */
    static unsigned char *is_prime = NULL;    /* sieve bit array */
    static size_t prime_count = 0;            /* number of primes */
    static idx_t current_limit = 0;           /* highest limit cached    */

    /* If we already have a large enough sieve, just return it. */
    if (limit <= current_limit) {
        *primes_out = primes;
        *cnt_out   = prime_count;
        return;
    }

    /* Free old data if any. */
    if (is_prime) free(is_prime);
    if (primes)   free(primes);

    if (limit < 2) {
        *primes_out = NULL;
        *cnt_out    = 0;
        current_limit = limit;
        return;
    }

    /* Allocate sieve.  The cast is needed for C++ compilation. */
    is_prime = (unsigned char *)calloc((size_t)limit + 1, 1);
    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = 1;

    for (idx_t i = 2; (idx_t)i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    /* Count primes and build the list. */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) ++cnt;
    primes = (idx_t *)malloc(cnt * sizeof(idx_t));
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) primes[idx++] = i;

    prime_count    = cnt;
    current_limit  = limit;

    *primes_out = primes;
    *cnt_out    = prime_count;
}

/*=== Legendre exponent of a prime in m! ==================================*/
static inline idx_t fact_exp(idx_t m, idx_t p)
{
    idx_t e = 0;
    while (m) {
        m /= p;
        e += m;
    }
    return e;
}

/*=== Sequence function a(n) =============================================*/
/* Returns the nth term of OEIS A295433.  The same code compiles for a
 * built‑in 8/16/32/64/128 bit unsigned type or for an arbitrary‑precision
 * bigint class that supports the usual arithmetic operators. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Build primes up to 12*n once. */
    idx_t limit = 12 * n;
    idx_t *primes = NULL;
    size_t prime_cnt = 0;
    ensure_primes(limit, &primes, &prime_cnt);

    seq_t result = 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        /* Compute exponent of this prime in the multinomial ratio. */
        idx_t e = fact_exp(12 * n, p)      /* (12n)!   */
              + fact_exp(n, p)            /* + n!      */
              - fact_exp(8 * n, p)       /* - (8n)!   */
              - fact_exp(3 * n, p)       /* - (3n)!   */
              - fact_exp(2 * n, p);      /* - (2n)!   */

        /* Multiply by p^e. */
        for (idx_t k = 0; k < e; ++k) {
            result *= (seq_t)p;
        }
    }
    return result;
}
