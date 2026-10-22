/*
 * A386326  Primes without {0, 8} as digits.
 * https://oeis.org/A386326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th prime that contains no decimal digits 0 or 8
 * Range:     u8 n <= 47, u16 n <= 3238, u32 n <= 3417, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digits, is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates primes sequentially, testing the digit condition,
   and stores the values in a static cache so that calls with arbitrary
   indices are answered correctly. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */
#include "../utils/get_digits.h"     /* void get_digits(seq_t x, uint8_t *buf, size_t *len); */
#include "../utils/seq_alloc.h"      /* seq_t *seq_alloc(size_t cap); */
#include "../utils/seq_realloc.h"    /* seq_t *seq_realloc(seq_t *old, size_t old_cap, size_t new_cap); */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n < 1 – harmless */

    /* Static cache of found primes */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;
    static size_t len     = 0;
    static uint64_t cand   = 2;      /* next integer to test */

    /* Grow cache until we have at least n values */
    while (len < n) {
        /* Test primality */
        if (is_prime_big((seq_t)cand)) {
            /* Check that none of its decimal digits are 0 or 8 */
            uint8_t buf[32];  /* enough for 128‑bit numbers */
            size_t dlen;
            get_digits((seq_t)cand, buf, &dlen);

            int ok = 1;
            for (size_t i = 0; i < dlen; ++i) {
                if (buf[i] == 0 || buf[i] == 8) {
                    ok = 0;
                    break;
                }
            }

            if (ok) {
                /* Allocate or grow the cache array */
                if (cache == NULL) {
                    cap = 8;
                    cache = seq_alloc(cap);
                } else if (len >= cap) {
                    size_t new_cap = cap << 1;
                    seq_t *tmp = seq_realloc(cache, cap, new_cap);
                    if (tmp == NULL) {   /* allocation failed */
                        exit(1);
                    }
                    cache = tmp;
                    cap = new_cap;
                }
                cache[len++] = (seq_t)cand;   /* store the prime */
            }
        }
        ++cand;  /* next integer to test */
    }

    return cache[n - 1];
}
