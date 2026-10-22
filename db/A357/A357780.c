/*
 * A357780  Primes p such that changing, in p, all 1's to 2's we get
 *          semiprimes and changing all 1's to 3's we get triprimes.
 * https://oeis.org/A357780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 283, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_prime_factors, next_prime, seq_alloc, seq_realloc,
 *            substitute_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: iterate over primes, substitute all decimal 1's with 2 and 3,
   and keep those whose substitutions have exactly 2 and 3 prime factors
   (with multiplicity).  Results are cached for future calls. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/next_prime.h"
#include "../utils/count_prime_factors.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/substitute_digit.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1 */

    /* cached sequence values */
    static seq_t *list = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static uint64_t start = 2;      /* next candidate for next_prime */

    while (len < n) {
        /* obtain next prime candidate */
        uint64_t p64 = next_prime(start);
        start = p64 + 1;           /* advance search start */
        seq_t p = (seq_t)p64;       /* prime as seq_t */

        seq_t p2 = substitute_digit(p, 2);
        seq_t p3 = substitute_digit(p, 3);

        seq_t f2 = count_prime_factors(p2);
        seq_t f3 = count_prime_factors(p3);

        if (f2 == (seq_t)2 && f3 == (seq_t)3) {
            /* grow cache if necessary */
            if (len >= cap) {
                size_t new_cap = cap ? cap * 2 : 8;
                seq_t *tmp = seq_realloc(list, cap, new_cap);
                if (!tmp) return (seq_t)0;   /* allocation failed */
                list = tmp;
                cap  = new_cap;
            }
            list[len++] = p;
        }
    }
    return list[n - 1];
}
