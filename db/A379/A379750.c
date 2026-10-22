/*
 * A379750  First prime of cousin prime pairs which differ, in their binary
 *          representation, by a single bit.
 * https://oeis.org/A379750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A379750: first prime of cousin prime pairs whose binary representations differ by a single bit.
   Method: iterate odd integers, keep those p where p and p+4 are prime and p % 8 == 1 or 3.  The result
   sequence is cached for repeated queries. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static inline bool seq_is_prime(seq_t n) {
    return is_prime((idx_t)n);
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;        /* undefined for offset 0 */

    static seq_t *cache = NULL;
    static size_t cap  = 0;
    static idx_t  count = 0;
    static seq_t  next  = (seq_t)3;

    while (count < n) {
        /* Find next qualifying prime */
        while (true) {
            if (seq_is_prime(next) &&
                seq_is_prime(next + (seq_t)4) &&
                (next % (seq_t)8) < (seq_t)4) {   /* 1 or 3 mod 8 */
                break;
            }
            next += (seq_t)2;    /* skip to next odd number */
        }

        /* Grow cache if necessary */
        if (count >= cap) {
            size_t new_cap = cap ? cap << 1 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* allocation failed – not expected */
            cache = tmp;
            cap   = new_cap;
        }

        cache[count++] = next;   /* store the term */
        next += (seq_t)2;        /* prepare next candidate */
    }
    return cache[n - 1];
}
