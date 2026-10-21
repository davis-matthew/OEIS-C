/*
 * A246932  a(n) = prime(15*n).
 * https://oeis.org/A246932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(15*n)
 * Range:     u8 n <= 3, u16 n <= 436, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

/* static state: list of primes up to prime_limit */
static uint64_t *prime_list = NULL;
static size_t prime_count  = 0;
static idx_t  prime_limit  = 0;

/* ensure that at least n primes are available */
static void ensure_n_primes(idx_t n)
{
    if (n <= prime_count)
        return;
    /* compute an upper bound for the n-th prime */
    idx_t lim = _prime_limit_for_idx(n);
    uint64_t *new_list;
    size_t new_cnt;
    build_primes_upto(lim, &new_list, &new_cnt);
    free(prime_list);
    prime_list  = new_list;
    prime_count = new_cnt;
    prime_limit = lim;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence; guard against 0 */
    if (n == 0)
        return 0;
    /* index of the desired prime is 15*n */
    idx_t target = n * 15ULL;
    ensure_n_primes(target);
    return (seq_t)prime_list[target - 1];
}
