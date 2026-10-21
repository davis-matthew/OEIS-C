/*
 * A285596  0-limiting word of the morphism 0->11, 1-> 0111.
 * https://oeis.org/A285596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Generation of the 0-limiting word of the morphism 0 -> 11, 1 ->
 *            0111. The function returns the nth symbol (a(1)=0, a(2)=1, …) by
 *            iteratively expanding from the initial symbol 0 until the
 *            desired index is reached
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Allow a graceful fallback for n == 0 (outside the offset) */
    if (n == 0) return (seq_t)0;

    size_t target = (size_t)(n - 1);          /* 0‑based index */

    /* Image strings for the morphism, encoded as 0/1 bytes. */
    static const unsigned char IMG0[8]  = {0,1,1,1,0,1,1,1};   /* 0 → 01110111 */
    static const unsigned char IMG1[14] = {1,1,0,1,1,1,0,1,1,1,0,1,1,1}; /* 1 → 11011101110111 */

    /* Mutable buffer holding the current prefix of the infinite word. */
    static unsigned char *buf  = NULL;
    static size_t        buflen = 0;

    /* Initialise with the base symbol 0 if not already done. */
    if (buf == NULL) {
        buf = (unsigned char*)malloc(1);
        if (!buf) return (seq_t)0;   /* allocation failure – fallback */
        buf[0]  = 0;
        buflen = 1;
    }

    /* Expand until the buffer contains the desired index. */
    while (buflen <= target) {
        size_t newlen = 0;
        size_t newcap = buflen * 14;  /* worst‑case growth */
        unsigned char *newbuf = (unsigned char*)malloc(newcap);
        if (!newbuf) return (seq_t)0; /* allocation failure – fallback */

        for (size_t i = 0; i < buflen; ++i) {
            if (buf[i] == 0) {
                memcpy(newbuf + newlen, IMG0, 8);
                newlen += 8;
            } else {
                memcpy(newbuf + newlen, IMG1, 14);
                newlen += 14;
            }
        }

        free(buf);
        buf    = newbuf;
        buflen = newlen;
    }

    return (seq_t)buf[target];
}
