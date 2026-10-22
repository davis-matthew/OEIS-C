/*
 * A341700  Sum of the primes p satisfying n < p <= 2n.
 * https://oeis.org/A341700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of primes p with n < p <= 2n (OEIS A341700)
 * Range:     u8 n <= 26, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static idx_t            sieve_limit = 0;   /* current sieve upper bound   */
static uint8_t         *sieve = NULL;      /* 0/1 flag for each number    */
static uint64_t        *prime_list = NULL; /* list of primes up to limit  */
static size_t          prime_cnt   = 0;    /* number of primes in list    */
static size_t          prime_cap   = 0;    /* allocated capacity          */

static void expand_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;   /* already sufficient */

    /* free previous sieve */
    if (sieve) {
        free(sieve);
        sieve = NULL;
    }
    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
    }

    /* allocate new sieve */
    sieve = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    if (!sieve)
        return; /* allocation failed; caller may handle it */

    /* mark all as potential primes */
    for (idx_t i = 0; i <= limit; ++i)
        sieve[i] = 1;
    if (limit >= 0) sieve[0] = 0;
    if (limit >= 1) sieve[1] = 0;

    /* classic Eratosthenes */
    for (idx_t p = 2; (p * p) <= limit; ++p) {
        if (sieve[p]) {
            for (idx_t q = p * p; q <= limit; q += p)
                sieve[q] = 0;
        }
    }

    /* build prime list */
    prime_cap = limit + 1; /* safe upper bound */
    prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    if (!prime_list)
        return; /* allocation failed */

    prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (sieve[i]) {
            prime_list[prime_cnt++] = (uint64_t)i;
        }
    }

    sieve_limit = limit; /* store new upper bound */
}

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1, but guard against undefined calls */
        return 0;

    idx_t limit = 2 * n;
    expand_sieve(limit);

    seq_t sum = 0;
    size_t i = 0;

    /* skip primes <= n */
    while (i < prime_cnt && prime_list[i] <= n)
        ++i;

    /* sum primes <= limit (i.e., <= 2n) */
    while (i < prime_cnt && prime_list[i] <= limit) {
        sum += (seq_t)prime_list[i];
        ++i;
    }

    return sum;
}
