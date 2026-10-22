/*
 * A329356  The binary expansion of a(n) is the first n terms of 2 - A000002.
 * https://oeis.org/A329356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((1 - c/2) * 2^n) where c is the Kolakoski constant.
 *            The binary expansion of a(n) is the first n terms of 2 – A000002
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Allocate array to store the generated Kolakoski sequence terms
       (values are 1 or 2).  n is small enough that malloc is inexpensive. */
    uint8_t *seq = (uint8_t *)malloc((size_t)n);
    if (!seq)
        return 0; /* fallback – unlikely for n <= 10000 */

    seq[0] = 1;
    if (n > 1)
        seq[1] = 2;

    seq_t acc = 0;             /* the result a(n) */
    idx_t pos = 0;             /* index in seq from which run length is read */
    int val = 1;               /* current value to output (1 or 2) */
    int remaining = seq[0];    /* how many times to output val */

    for (idx_t idx = 0; idx < n; ++idx) {
        /* s[idx] is 1 iff the current sequence value is 1 */
        uint8_t bit = (uint8_t)(val == 1);
        acc = (acc << 1) | bit;

        seq[idx] = (uint8_t)val;       /* store the generated term */

        --remaining;
        if (remaining == 0 && idx + 1 < n) {
            ++pos;                      /* advance to next run length */
            val = 3 - val;              /* toggle between 1 and 2 */
            remaining = seq[pos];
        }
    }

    free(seq);
    return acc;
}
