/*
 * A267379  Positions of 1's in A094186.
 * https://oeis.org/A267379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th 1 in A094186, the word that starts
 *            0,1 and then appends the prefix of length k for k = 1,2,3,...
 *            (computed by generating the word)
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9923 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static unsigned char *w = NULL;
    static size_t wlen = 0, wcap = 0;
    static size_t *pos = NULL;
    static size_t pcnt = 0, pcap = 0;
    static size_t next_k = 1;
    static size_t scanned = 0;

    if (w == NULL) {
        wcap = 64;
        w = (unsigned char *)malloc(wcap);
        pcap = 64;
        pos = (size_t *)malloc(pcap * sizeof(size_t));
        w[0] = 0; w[1] = 1; wlen = 2;
    }

    while (pcnt < (size_t)n) {
        /* scan any unscanned part of the word */
        while (scanned < wlen) {
            if (w[scanned] == 1) {
                if (pcnt == pcap) {
                    pcap *= 2;
                    pos = (size_t *)realloc(pos, pcap * sizeof(size_t));
                }
                pos[pcnt++] = scanned + 1;
            }
            scanned++;
        }
        if (pcnt >= (size_t)n) break;
        /* append prefix of length next_k */
        while (wlen + next_k > wcap) {
            wcap *= 2;
            w = (unsigned char *)realloc(w, wcap);
        }
        for (size_t i = 0; i < next_k; ++i) w[wlen + i] = w[i];
        wlen += next_k;
        next_k++;
    }
    return (seq_t)pos[n - 1];
}
