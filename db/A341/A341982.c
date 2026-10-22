/*
 * A341982  Number of ways to write n as an ordered sum of 2 primes (counting
 *          1 as a prime).
 * https://oeis.org/A341982
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered sums of two primes (including 1 as a
 *            prime) that equal n (a(n) = |{ (p,q) : p,q are primes, p+q = n
 *            }|, with 1 treated as prime)
 * Range:     u8 n <= 2729, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Static state for the prime sieve */
static idx_t        _prime_limit       = 0;          /* current sieve boundary   */
static uint8_t *    _prime_flag        = NULL;       /* flags for each index     */
static uint64_t *   _prime_list        = NULL;       /* list of primes ≤ limit   */
static size_t       _prime_count       = 0;          /* number of primes found   */

/* Ensure that primes up to at least 'limit' are available. */
static void
_sieve_primes(idx_t limit)
{
    if (limit <= _prime_limit)
        return;                         /* already sufficient      */

    /* Allocate fresh flag array (all 1 initially). */
    size_t sz = (size_t)limit + 1;
    uint8_t *new_flag = (uint8_t *)calloc(sz, 1);
    if (!new_flag)
        return;                         /* allocation failed        */

    /* Initialize all indices as potential primes */
    for (idx_t i = 2; i <= limit; ++i)
        new_flag[i] = 1;

    new_flag[0] = 0;                    /* 0 is not prime           */

    /* Eratosthenes sieve for indices ≥2. */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (new_flag[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                new_flag[j] = 0;
        }
    }

    new_flag[1] = 1;                    /* 1 is treated as prime      */

    /* Count primes to allocate the list. */
    size_t pc = 0;
    for (idx_t i = 1; i <= limit; ++i)
        if (new_flag[i])
            ++pc;

    uint64_t *new_list = (uint64_t *)malloc(pc * sizeof(uint64_t));
    if (!new_list) {
        free(new_flag);
        return;                         /* allocation failed        */
    }

    /* Populate the prime list. */
    size_t pos = 0;
    for (idx_t i = 1; i <= limit; ++i)
        if (new_flag[i])
            new_list[pos++] = (uint64_t)i;

    /* Release old data and replace with new data. */
    free(_prime_flag);
    free(_prime_list);
    _prime_flag   = new_flag;
    _prime_list   = new_list;
    _prime_count  = pc;
    _prime_limit  = limit;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                        /* below offset */

    _sieve_primes(n);                    /* ensure primes ≤ n   */

    seq_t count = 0;
    for (size_t i = 0; i < _prime_count; ++i) {
        uint64_t p = _prime_list[i];
        if (p > n)
            break;
        idx_t m = n - (idx_t)p;
        if (m <= (idx_t)_prime_limit && _prime_flag[m])
            ++count;
    }
    return count;
}
