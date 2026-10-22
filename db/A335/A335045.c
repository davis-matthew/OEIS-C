/*
 * A335045  Minimal common prime of two Goldbach partitions of 2n and 2(n+1)
 *          or zero if no common prime exists.
 * https://oeis.org/A335045
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p such that 2·n−p and 2·(n+1)−p are both
 *            primes, or 0 if no such p exists (oeis A335045)
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --- internal prime sieve state ----------------------------------------- */

static idx_t      prime_limit  = 0;      /* highest number currently sieved   */
static uint64_t * prime_list   = NULL;  /* array of primes ≤ prime_limit     */
static size_t    prime_count   = 0;      /* number of primes in prime_list    */
static char *    sieve_flags  = NULL;   /* 0 = prime, 1 = composite         */

/* ensure that prime_limit ≥ limit; if not, rebuild the sieve   */
static void _ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;   /* already sufficient */

    /* allocate fresh sieve flags */
    char *new_flags = (char *)calloc((size_t)(limit + 1), 1);
    if (!new_flags)
        return;   /* allocation failure – leave incomplete data */

    /* simple Eratosthenes sieve */
    for (idx_t i = 2; (idx_t)(i * i) <= limit; ++i)
        if (!new_flags[i])
            for (idx_t j = i * i; j <= limit; j += i)
                new_flags[j] = 1;

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!new_flags[i])
            ++cnt;

    /* build prime list */
    uint64_t *new_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_list) {       /* allocation failure – clean up */
        free(new_flags);
        return;
    }

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!new_flags[i])
            new_list[idx++] = (uint64_t)i;

    /* replace old data */
    free(prime_list);
    free(sieve_flags);
    prime_list   = new_list;
    prime_count  = cnt;
    sieve_flags  = new_flags;
    prime_limit  = limit;
}

/* primality test for numbers up to prime_limit using the sieve;
   if number > prime_limit, fall back to trial division              */
static bool _is_prime_small(idx_t x)
{
    if (x < 2)
        return false;
    if (x <= prime_limit)
        return sieve_flags[x] == 0;

    /* trial division up to sqrt(x) */
    for (idx_t i = 2; (idx_t)(i * i) <= x; ++i)
        if (x % i == 0)
            return false;
    return true;
}

/* ------------------------------------------------------------------------ */
/*  Main sequence function                                                */
/* --- the function must have external linkage (no static) ---------------- */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;          /* below the official offset */

    /* we need primes up to 2·(n+1) to test the two Goldbach partitions   */
    _ensure_primes(2 * (n + 1));

    idx_t max_p = 2 * n - 2;   /* any larger prime would make 2·n−p < 2       */

    for (size_t i = 0; i < prime_count && prime_list[i] <= max_p; ++i) {
        idx_t p  = prime_list[i];
        idx_t s1 = 2 * n - p;          /* partner in partition of 2n   */
        idx_t s2 = 2 * (n + 1) - p;    /* partner in partition of 2(n+1) */

        if (s1 >= 2 && s2 >= 2 &&
            _is_prime_small(s1) && _is_prime_small(s2))
            return (seq_t)p;            /* minimal p found */
    }

    return (seq_t)0;      /* no common prime for this n */
}
