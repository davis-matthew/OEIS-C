/*
 * A373601  a(n) = 1 if the sum of prime factors (with multiplicity) of
 *          A276086(n) is a multiple of 3, otherwise 0, where A276086 is the
 *          primorial base exp-function.
 * https://oeis.org/A373601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the sum of the prime factors (with multiplicity) of
 *            A276086(n) is a multiple of 3, otherwise 0
 * Range:     u8 n <= 979, u16 n <= 9811, u32 n <= 9811, u64 n <= 9811,
 *            u128 n <= 9811, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------ */
/* Lazy prime table for the first ~18000 primes (up to 200000). */
/* ------------------------------------------------------------ */
static uint64_t *prime_tbl = NULL;
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (prime_tbl != NULL)
        return;

    const idx_t LIMIT = 200000;          /* enough for n <= 10000      */
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) {
        /* In an extreme memory-constraint case do nothing */
        return;
    }
    memset(is_prime, true, (LIMIT + 1) * sizeof(bool));
    if (LIMIT >= 0) {
        is_prime[0] = false;
        if (LIMIT >= 1) /* LIMIT >= 1 because LIMIT = 200000 */
            is_prime[1] = false;
    }
    for (idx_t i = 2; (i * i) <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            ++cnt;

    prime_tbl = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!prime_tbl) {
        free(is_prime);
        return;
    }

    size_t idx = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            prime_tbl[idx++] = (uint64_t)i;

    prime_cnt = cnt;
    free(is_prime);
}

/* ------------------------------------------------------------ */
/* Sequence a(n) implementation                                 */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_primes();

    seq_t sum_mod3 = 0;                 /* running sum modulo 3       */
    seq_t n_copy   = (seq_t)n;          /* work copy of n             */
    idx_t pos      = 0;                 /* index into the prime table */

    while (n_copy != 0) {
        seq_t prime     = (seq_t)prime_tbl[pos];
        seq_t digit     = n_copy % prime;
        seq_t digit_mod = digit % 3;
        seq_t prime_mod = prime  % 3;

        /* Add (digit * prime) modulo 3 without overflow */
        sum_mod3 += (digit_mod * prime_mod) % 3;
        sum_mod3 %= 3;

        n_copy /= prime;
        ++pos;
    }

    return (seq_t)(sum_mod3 == 0);      /* 1 if multiple of 3, else 0 */
}
