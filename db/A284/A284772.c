/*
 * A284772  Fixed point of the morphism 0 -> 01, 1 -> 0010.
 * https://oeis.org/A284772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th bit (1‑based) of the fixed point of the morphism
 *            0 → 01, 1 → 0010
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static buffer holding the current word (as bytes) */
    static uint8_t *w = NULL;
    static size_t len = 0;

    /* initialise with the root symbol '0' */
    if (w == NULL)
    {
        w = (uint8_t *)malloc(sizeof(uint8_t));
        if (!w)
            return (seq_t)0;  /* allocation failure – unlikely */
        w[0] = 0;
        len = 1;
    }

    /* expand until the word is long enough to cover index n */
    while (len < (size_t)n)
    {
        size_t new_len = 0;
        /* first pass: compute new length */
        for (size_t i = 0; i < len; ++i)
            new_len += (w[i] == 0U) ? 2U : 4U;

        uint8_t *new_w = (uint8_t *)malloc(new_len);
        if (!new_w)
            return (seq_t)0;  /* allocation failure */

        /* second pass: fill the new word */
        size_t pos = 0;
        for (size_t i = 0; i < len; ++i)
        {
            if (w[i] == 0U)
            {
                new_w[pos++] = 0U;
                new_w[pos++] = 1U;
            }
            else  /* w[i] == 1U */
            {
                new_w[pos++] = 0U;
                new_w[pos++] = 0U;
                new_w[pos++] = 1U;
                new_w[pos++] = 0U;
            }
        }

        free(w);
        w = new_w;
        len = new_len;
    }

    return (seq_t)w[(size_t)(n - 1)];
}
