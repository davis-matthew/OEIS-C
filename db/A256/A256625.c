/*
 * A256625  Product of the first n lucky numbers.
 * https://oeis.org/A256625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{k=1..n} A000959(k) (product of the first n lucky
 *            numbers)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   generate_lucky_numbers
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/generate_lucky_numbers.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Give the algorithm a reasonable starting bound.  For n ≤ 10 000
     * this is more than enough, but we will double until it is. */
    idx_t estimate = (idx_t)(n * 10);
    if (estimate < 1000)
        estimate = 1000;

    /* Allocate a buffer that can hold the odds up to the current
     * estimate.  Once it is allocated, it is reused. */
    size_t buffer_size = (size_t)((estimate + 1) / 2);
    uint32_t *buffer = (uint32_t *)malloc(buffer_size * sizeof(uint32_t));
    if (!buffer)
        return (seq_t)0;   /* allocation failure */

    while (1) {
        /* Ensure buffer is large enough for the current estimate. */
        size_t needed = (size_t)((estimate + 1) / 2);
        if (needed > buffer_size) {
            uint32_t *tmp = (uint32_t *)realloc(buffer, needed * sizeof(uint32_t));
            if (!tmp) {
                free(buffer);
                return (seq_t)0;
            }
            buffer = tmp;
            buffer_size = needed;
        }

        size_t lucky_len;
        generate_lucky_numbers(estimate, buffer, &lucky_len);

        if (lucky_len >= (size_t)n) {
            seq_t prod = (seq_t)1;
            for (size_t k = 0; k < (size_t)n; ++k)
                prod = prod * (seq_t)buffer[k];
            free(buffer);
            return prod;
        }

        /* Not enough lucky numbers: increase estimate and retry. */
        estimate *= 2;
    }
}
