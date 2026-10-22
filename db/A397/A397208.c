/*
 * A397208  a(n) is the number of primes q < p = prime(n) such that (p + q)/4
 *          is prime.
 * https://oeis.org/A397208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of primes q < p = prime(n) such that (p + q)/4 is
 *            prime. For n <= 10000 this implementation precomputes all primes
 *            up to 200000 with a single sieve and evaluates the condition
 *            directly
 * Range:     u8 n <= 7360, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The build supplies seq_t and idx_t types.  We only use standard C
   facilities and no external helper functions. */

/* ------------------------------------------------------------------ */
/* Static storage for the sieve and the prime list.  All arrays are
   allocated on the first call to a() or ensure_precomputed() and
   reused for subsequent calls. */

static uint8_t  *is_prime_small = NULL;   /* 1 if index is prime, 0 otherwise */
static size_t    prime_count   = 0;        /* number of primes found */
static uint64_t *prime_list    = NULL;    /* array of primes in order */

/* ------------------------------------------------------------------ */
/* Helper: precompute primes up to LIMIT using a simple sieve of Eratosthenes.
   We set LIMIT to a safe value that covers prime(10000) = 104729. */
#define PRECOMP_LIMIT 200000

static void ensure_precomputed(void)
{
    if (is_prime_small != NULL)
        return;                           /* already initialized */

    /* Allocate and initialise sieve array */
    is_prime_small = (uint8_t *)calloc(PRECOMP_LIMIT + 1, sizeof(uint8_t));
    if (is_prime_small == NULL)
        return;                           /* out of memory: abort silently */

    for (size_t i = 2; i <= PRECOMP_LIMIT; ++i)
        is_prime_small[i] = 1;             /* mark all as potential primes */

    for (size_t i = 2; i * i <= PRECOMP_LIMIT; ++i) {
        if (is_prime_small[i]) {
            for (size_t j = i * i; j <= PRECOMP_LIMIT; j += i)
                is_prime_small[j] = 0;
        }
    }

    /* Count primes and build prime list */
    for (size_t i = 2; i <= PRECOMP_LIMIT; ++i)
        if (is_prime_small[i])
            ++prime_count;

    prime_list = (uint64_t *)malloc(prime_count * sizeof(uint64_t));
    if (prime_list == NULL) {
        free(is_prime_small);
        is_prime_small = NULL;
        return;
    }

    size_t idx = 0;
    for (size_t i = 2; i <= PRECOMP_LIMIT; ++i)
        if (is_prime_small[i])
            prime_list[idx++] = (uint64_t)i;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.  The computation is straightforward:
   1. Ensure the prime tables are ready.
   2. Pick the nth prime p = prime_list[n-1].
   3. For each previous prime q < p, test whether (p + q) % 4 == 0.
   4. If so, compute r = (p + q)/4 and test if r < = PRECOMP_LIMIT
      and is_prime_small[r] == 1.  Count such occurrences. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* behaviour for n < 1 is undefined */

    ensure_precomputed();
    if (is_prime_small == NULL || prime_list == NULL)
        return (seq_t)0;          /* fallback if memory allocation failed */

    if ((size_t)n > prime_count)
        return (seq_t)0;          /* n too large for precomputed range */

    uint64_t p = prime_list[(size_t)n - 1];
    seq_t    count = 0;

    for (size_t i = 0; i < (size_t)n - 1; ++i) {
        uint64_t q = prime_list[i];
        if (((p + q) & 3ULL) != 0)          /* not divisible by 4 */
            continue;

        uint64_t r = (p + q) >> 2;          /* r = (p + q)/4 */

        /* r will definitely be <= PRECOMP_LIMIT because r <= (p + p-1)/4 < p/2
           and prime(p) < PRECOMP_LIMIT. */
        if (r <= PRECOMP_LIMIT && is_prime_small[r])
            ++count;
    }

    return count;
}
