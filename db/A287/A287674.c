/*
 * A287674  {0->1, 1->001}-transform of the infinite Fibonacci word A003849.
 * https://oeis.org/A287674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_bit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the nth bit of the word obtained by
 * replacing each 0 of the infinite Fibonacci word (A003849) by 1
 * and each 1 by 001.
 *
 * Implementation: we build the Fibonacci word on demand using the
 * morphism 0→01, 1→0 and simultaneously generate the transformed
 * sequence.  The produced terms are cached so that later calls require
 * no recomputation.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/append_bit.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)   /* off‑by‑one guard, though offset is 1   */
        return (seq_t)0;

    /* cached transformed sequence */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0, seq_len = 0;

    /* Fibonacci word cache */
    static uint8_t *fib = NULL;
    static size_t fib_len = 0;
    static idx_t fib_processed = 0;

    /* first call: initialise the Fibonacci word with the single symbol 0 */
    if (fib_len == 0) {
        fib_len = 1;
        fib = (uint8_t *)malloc(fib_len);
        fib[0] = 0;
        fib_processed = 0;
    }

    /* extend the transformed sequence until it contains position n   */
    while (seq_len < (size_t)n) {
        /* if all current Fibonacci bits are consumed, expand the word  */
        if (fib_processed >= (idx_t)fib_len) {
            size_t new_len = 0;
            for (size_t i = 0; i < fib_len; ++i)
                new_len += (fib[i] == 0) ? 2 : 1;   /* 0→01 (2), 1→0 (1) */

            uint8_t *new_fib = (uint8_t *)malloc(new_len);
            size_t pos = 0;
            for (size_t i = 0; i < fib_len; ++i) {
                if (fib[i] == 0) {          /* 0 → 0 1 */
                    new_fib[pos++] = 0;
                    new_fib[pos++] = 1;
                } else {                    /* 1 → 0   */
                    new_fib[pos++] = 0;
                }
            }
            free(fib);
            fib = new_fib;
            fib_len = new_len;  /* the new word is a longer prefix of the same limit */
            /* fib_processed stays unchanged – it points into the new word */
        }

        /* take the next Fibonacci bit */
        uint8_t b = fib[fib_processed++];
        if (b == 0) {
            append_bit(&seq, &seq_cap, &seq_len, (seq_t)1);
        } else {                     /* b == 1  → 0 0 1 */
            append_bit(&seq, &seq_cap, &seq_len, (seq_t)0);
            append_bit(&seq, &seq_cap, &seq_len, (seq_t)0);
            append_bit(&seq, &seq_cap, &seq_len, (seq_t)1);
        }
    }

    return seq[n - 1];
}
