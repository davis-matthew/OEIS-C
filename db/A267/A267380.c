/*
 * A267380  First differences of A267379.
 * https://oeis.org/A267380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + length of the (n+1)-th run of 0's in A094186; the
 *            word is built by starting with 0,1 and appending the prefix of
 *            length k for k = 1,2,3,
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 145 terms through n = 9662 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static unsigned char *w = NULL;
    static size_t wlen = 0, wcap = 0;
    static size_t nextk = 1;
    static size_t scan = 0;
    static size_t cur = 0;
    static unsigned *runs = NULL;
    static size_t rlen = 0, rcap = 0;

    if (w == NULL) {
        wcap = 64;
        w = (unsigned char *)malloc(wcap);
        w[0] = 0; w[1] = 1;
        wlen = 2;
        rcap = 64;
        runs = (unsigned *)malloc(rcap * sizeof(unsigned));
    }

    while ((idx_t)rlen <= n) {
        /* extend word if everything has been scanned */
        if (scan >= wlen) {
            while (wlen + nextk > wcap) {
                wcap *= 2;
                w = (unsigned char *)realloc(w, wcap);
            }
            for (size_t i = 0; i < nextk; ++i) w[wlen + i] = w[i];
            wlen += nextk;
            ++nextk;
        }
        /* scan new part */
        while (scan < wlen && (idx_t)rlen <= n) {
            if (w[scan] == 0) {
                ++cur;
            } else {
                if (rlen >= rcap) {
                    rcap *= 2;
                    runs = (unsigned *)realloc(runs, rcap * sizeof(unsigned));
                }
                runs[rlen++] = (unsigned)cur;
                cur = 0;
            }
            ++scan;
        }
    }
    return (seq_t)runs[n] + 1;
}
