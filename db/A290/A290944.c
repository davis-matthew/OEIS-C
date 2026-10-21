/*
 * A290944  Primes p such that sum of digits of p^3 is a perfect square.
 * https://oeis.org/A290944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, digit_sum, floor_sqrt, next_prime, seq_alloc,
 *            seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: iterate primes and collect those where the sum of digits of p^3 is a perfect square */

#include <stddef.h>
#include <stdbool.h>

#include "../utils/next_prime.h"
#include "../utils/big_pow.h"
#include "../utils/digit_sum.h"
#include "../utils/floor_sqrt.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache  = NULL;      /* array of found primes */
    static size_t  cap   = 0;          /* allocated capacity */
    static size_t  len   = 0;          /* number of primes stored */
    static uint64_t last_checked = 0;  /* last prime examined */

    /* expand cache until we have at least n primes */
    while (len < n) {
        /* get next prime */
        uint64_t prime = next_prime(last_checked);
        last_checked = prime;

        /* compute p^3 and sum of its digits */
        seq_t cube = big_pow((seq_t)prime, (seq_t)3);
        seq_t sum  = digit_sum(cube);

        /* check whether sum is a perfect square */
        seq_t root = floor_sqrt(sum);
        if (root * root == sum) {
            /* ensure capacity */
            if (len == cap) {
                size_t new_cap = (cap == 0) ? 64 : cap << 1;
                seq_t *tmp = seq_realloc(cache, cap, new_cap);
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = (seq_t)prime;
        }
    }
    /* 1‑based indexing */
    return cache[n - 1];
}
