/*
 * A276660  Primes of the form p*2^k - 1 where p is an odd prime and k >= 0.
 * https://oeis.org/A276660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A276660: primes of the form p*2^k – 1, where p is an odd prime and k >= 0;
   computed by an incremental search over the integers. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *terms = NULL;
    static idx_t term_cnt = 0;   /* number of terms stored */
    static size_t term_cap = 0;  /* current capacity of the cache */
    static idx_t candidate = 2;  /* next integer to test */

    /* grow the cache when needed */
    if (term_cnt >= term_cap) {
        size_t new_cap = (term_cap == 0) ? 64 : term_cap << 1;
        seq_t *tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        terms = tmp;
        term_cap = new_cap;
    }

    /* generate terms until we have at least n of them */
    while (term_cnt < n) {
        seq_t m = (seq_t)candidate;

        if (is_prime_big(m)) {
            seq_t mp1 = m + 1;
            while ((mp1 & 1) == 0) mp1 >>= 1;
            if (mp1 > 1 && is_prime_big(mp1)) {
                terms[term_cnt++] = m;
            }
        }
        ++candidate;
    }

    return terms[n - 1];
}
