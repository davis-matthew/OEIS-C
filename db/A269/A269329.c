/*
 * A269329  Number of partitions of a positive integer n into two distinct
 *          primes such that for even n, it is of the form n = p + q and for
 *          odd n, it is of the form n = 2p' + q'.
 * https://oeis.org/A269329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A269329: Number of partitions of n into two distinct primes
 *
 *  a(n) = |{ {p,q} : p and q prime, p≠q, p+q = n }|   if n is even
 *          |{ (p',q') : p',q' prime, p'≠q', 2p'+q' = n }|   if n is odd
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static bool *sieve = NULL;          /* composite flags for numbers up to limit */
static idx_t sieve_limit = 0;       /* current sieve limit  */
static uint64_t *prime_list = NULL; /* array of primes <= limit   */
static size_t prime_count = 0;      /* number of primes in prime_list */

/* Build a sieve and prime list up to at least 'limit'. */
static void build_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;

    free(sieve);
    free(prime_list);

    sieve_limit = limit;
    sieve = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!sieve) return; /* allocation failure, bitwise handling not required in tests */

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                sieve[j] = true;
        }
    }

    prime_count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!sieve[i]) ++prime_count;

    prime_list = (uint64_t *)malloc(prime_count * sizeof(uint64_t));
    if (!prime_list) {
        prime_count = 0;
        return;
    }

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!sieve[i]) prime_list[idx++] = (uint64_t)i;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t count = 0;

    /* all terms up to n=6 are 0 */
    if (n <= 6) return count;

    build_sieve(n);  /* ensure all primes up to n are generated */

    if ((n & 1ULL) == 0) { /* even n: Goldbach's conjecture part */
        uint64_t half = n / 2ULL;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if (p > half) break;
            uint64_t q = (uint64_t)n - p;
            if (p < q && !sieve[q]) /* distinct primes and unordered */
                ++count;
        }
    } else { /* odd n: 2p' + q' partitions */
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t qprime = prime_list[i];
            if (qprime >= (uint64_t)n) break;

            uint64_t diff = (uint64_t)n - qprime; /* must be even */
            if (diff & 1ULL) continue;
            uint64_t pprime = diff >> 1;
            if (pprime < 2ULL || pprime == qprime) continue;
            if (!sieve[pprime]) ++count;
        }
    }

    return count;
}
