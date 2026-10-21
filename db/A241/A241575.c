/*
 * A241575  Sturmian expansion of 1/2 in base sqrt(2)-1.
 * https://oeis.org/A241575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 238, u16 n <= 238, u32 n <= 238, u64 n <= 238,
 *            u128 n <= 238, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fixed point of the morphism  
   0 → 01010  
   1 → 0101010   */
/* Returns the nth element (0‑based) of A241575. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of the generated word */
    static seq_t *buf = NULL;
    static size_t len = 0;  /* current length of buf */

    /* initialise with the seed word "0" */
    if (len == 0) {
        buf = (seq_t *)malloc(sizeof(seq_t));
        if (!buf) return 0;    /* out of memory: return 0 as a fallback */
        buf[0] = 0;
        len = 1;
    }

    /* generate sufficient length until len > n */
    while (len <= (size_t)n) {
        /* first compute the length of the next generation */
        size_t new_len = 0;
        for (size_t i = 0; i < len; ++i)
            new_len += (buf[i] == 0) ? 5 : 7;

        /* allocate buffer for the next generation */
        seq_t *new_buf = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_buf) return 0; /* out of memory */

        /* fill the new buffer with the morphism images */
        size_t pos = 0;
        for (size_t i = 0; i < len; ++i) {
            if (buf[i] == 0) {
                new_buf[pos++] = 0;
                new_buf[pos++] = 1;
                new_buf[pos++] = 0;
                new_buf[pos++] = 1;
                new_buf[pos++] = 0;
            } else {
                new_buf[pos++] = 0;
                new_buf[pos++] = 1;
                new_buf[pos++] = 0;
                new_buf[pos++] = 1;
                new_buf[pos++] = 0;
                new_buf[pos++] = 1;
                new_buf[pos++] = 0;
            }
        }

        /* discard old buffer and adopt the new one */
        free(buf);
        buf = new_buf;
        len = new_len;
    }

    /* return the required element */
    return buf[(size_t)n];
}
