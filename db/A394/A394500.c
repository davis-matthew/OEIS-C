/*
 * A394500  Ways to represent n = k + p where p is prime and k > 0 is not
 *          squarefree.
 * https://oeis.org/A394500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p < n such that (n - p) is not
 *            squarefree
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   build_primes_upto, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_squarefree.h"

/* Static state: primes up to the maximum n-1 requested so far. */
static idx_t prime_limit = 0;
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;

/* Grow the prime table to cover all primes <= new_limit. */
static void update_primes(idx_t new_limit) {
    if (prime_list) {
        free(prime_list);
    }
    uint64_t *tmp = NULL;
    size_t cnt = 0;
    build_primes_upto(new_limit, &tmp, &cnt);
    prime_list = tmp;
    prime_count = cnt;
    prime_limit = new_limit;
}

seq_t a(idx_t n) {
    /* a(1) = 0 by definition, treat n == 0 as 0 for safety */
    if (n <= 1) return (seq_t)0;

    /* Ensure prime table covers all primes < n. */
    if (prime_limit < n - 1) {
        update_primes(n - 1);
    }

    /* Count primes p < n with (n - p) non‑squarefree. */
    idx_t count = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p >= n) break;                /* primes are sorted */
        idx_t k = n - p;                  /* k > 0 */
        seq_t kk = (seq_t)k;              /* promote to seq_t for is_squarefree */
        if (!is_squarefree(kk)) {
            ++count;
        }
    }
    return (seq_t)count;
}
