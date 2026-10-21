/*
 * A284935  Fixed point of the morphism 0 -> 01, 1 -> 1100.
 * https://oeis.org/A284935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A284935: fixed point of the morphism 0 -> 01, 1 -> 1100. */
/* The term at position n (1‑based) is obtained by iteratively applying the
   morphism until the sequence reaches length n.  The sequence is
   3‑automatic, but for n up to about 10 000 a simple iterative generation
   is fast enough. */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined offset – guard */

    /* Target index in 0‑based form (fit into size_t for our limits). */
    idx_t target64 = n - 1;
    size_t target   = (size_t)target64;

    /* Allocate buffers that hold only 0 or 1 as uint8_t.  Using a
       plain byte array avoids calling constructors for arbitrary‑precision
       seq_t types. */
    size_t len = 1;
    uint8_t *buf   = (uint8_t *)malloc(len);
    if (!buf) return 0;                    /* unlikely failure */
    buf[0] = 0;                             /* initial fixed point "0" */

    /* Expand until we have at least target+1 symbols. */
    while (len <= target) {
        /* Each symbol expands to at most 4, plus a safety margin. */
        size_t newlen_est = len * 4 + 4;
        uint8_t *newbuf = (uint8_t *)malloc(newlen_est);
        if (!newbuf) { free(buf); return 0; }

        size_t newlen = 0;
        for (size_t i = 0; i < len; ++i) {
            if (buf[i] == 0) {              /* 0 -> 01 */
                newbuf[newlen++] = 0;
                newbuf[newlen++] = 1;
            } else {                        /* 1 -> 1100 */
                newbuf[newlen++] = 1;
                newbuf[newlen++] = 1;
                newbuf[newlen++] = 0;
                newbuf[newlen++] = 0;
            }
        }

        free(buf);
        buf  = newbuf;
        len  = newlen;
    }

    seq_t result = (seq_t)buf[target];   /* convert small int to seq_t */
    free(buf);
    return result;
}
