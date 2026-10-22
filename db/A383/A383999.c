/*
 * A383999  Sequence obtained by replacing 3-term subwords of A003849 by
 *          0,1,2,3 as described in Comments.
 * https://oeis.org/A383999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate the sequence A003849 by iteratively applying
   the morphism 0 → 01, 1 → 0, then replace each 3‑term window
   (0,0,1) → 0, (0,1,0) → 1, (1,0,0) → 2, (1,0,1) → 3. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* A static buffer holding the generated A003849 bits. */
    static uint8_t *bits  = NULL;
    static size_t bits_len = 0;

    /* We need bits at indices n-1, n, n+1 to extract the triple. */
    size_t needed = (size_t)(n + 2);  /* because indices start at 0 */

    if (bits_len < needed) {
        /* First call: initialise with the single symbol 0. */
        if (bits_len == 0) {
            bits_len = 1;
            bits = (uint8_t *)malloc(bits_len);
            bits[0] = 0;
        }

        /* Grow the sequence until it is long enough. */
        while (bits_len < needed) {
            /* Count zeros in the current sequence */
            size_t zeros = 0;
            for (size_t i = 0; i < bits_len; ++i)
                if (bits[i] == 0) ++zeros;

            size_t new_len = bits_len + zeros; /* each zero expands to 2, each one stays 1 */
            uint8_t *new_bits = (uint8_t *)malloc(new_len);
            size_t pos = 0;
            for (size_t i = 0; i < bits_len; ++i) {
                if (bits[i] == 0) {
                    new_bits[pos++] = 0;
                    new_bits[pos++] = 1;
                } else {            /* bits[i] == 1 */
                    new_bits[pos++] = 0;
                }
            }
            free(bits);
            bits = new_bits;
            bits_len = new_len;
        }
    }

    /* Extract the triple starting at position n-1 */
    uint8_t b1 = bits[(size_t)(n - 1)];
    uint8_t b2 = bits[(size_t)(n)];
    uint8_t b3 = bits[(size_t)(n + 1)];

    /* Map to 0,1,2,3 according to the specification */
    if (b1 == 0 && b2 == 0 && b3 == 1) return (seq_t)0;
    if (b1 == 0 && b2 == 1 && b3 == 0) return (seq_t)1;
    if (b1 == 1 && b2 == 0 && b3 == 0) return (seq_t)2;
    /* Remaining possibility: (1,0,1) */
    return (seq_t)3;
}
