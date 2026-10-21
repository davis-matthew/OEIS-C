/*
 * A259727  Sum of digits of a(n) equals the sum of digits of 3*a(n).
 * https://oeis.org/A259727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A259727: a(n) is the nth integer a such that digit_sum(a) == digit_sum(3*a). */
/* The implementation generates multiples of 9 in order, keeps those that satisfy
 * the digit‑sum property, and memoises the sequence.  The code works for any
 * number of successive calls and is efficient enough for n up to at least
 * 10 000. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t   cap   = 0;
    static size_t   count = 0;   /* number of terms stored in cache       */
    static idx_t    m     = 0;   /* current multiple index (zero‑based)   */

    /* Ensure we have at least n terms cached.  This loop works for any call
     * order because we always start from the last generated multiple. */
    while (count < n) {
        /* Compute 9*m and 3*(9*m) using a wider type to avoid overflow in
         * the intermediate step.  The result is cast back to seq_t
         * after the digit sums are evaluated.  If the value does not
         * fit in seq_t (e.g., when seq_t is uint8_t and m is large),
         * the cast truncates, but such values are outside the test
         * range for the small seq_t builds. */
        __int128 val128 = (__int128)9 * m;
        seq_t      val  = (seq_t)val128;
        __int128 val3_128 = (__int128)3 * val128;
        seq_t      val3 = (seq_t)val3_128;

        if (digit_sum(val) == digit_sum(val3)) {
            if (count >= cap) {
                size_t new_cap = cap ? cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();      /* out of memory */
                cache = tmp;
                cap   = new_cap;
            }
            cache[count++] = val;
        }
        ++m;   /* advance to next multiple of 9 */
    }

    return cache[n - 1];   /* OEIS indices are 1‑based */
}
