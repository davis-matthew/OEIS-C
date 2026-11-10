/*
 * A284370  Positions of 0 in A284369; complement of A284371.
 * https://oeis.org/A284370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   positions of 0 in the fixed point of 0->1, 1->1001 (A284369);
 *            computed by generating the word and recording zero positions
 * Range:     u8 n <= 108, u16 n <= 13376, u32 n <= 13376, u64 n <= 13376,
 *            u128 n <= 13376, big any n
 * Validated: standard, pass, 77 terms through n = 9469 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static unsigned char *W = NULL;
    static size_t len = 0, cap = 0, rd = 0, scanned = 0;
    static uint64_t *Z = NULL;
    static size_t zc = 0, zcap = 0;

    if (W == NULL) {
        cap = 1024;
        W = (unsigned char *)malloc(cap);
        W[0] = 1; W[1] = 0; W[2] = 0; W[3] = 1;
        len = 4; rd = 1; scanned = 0;
        zcap = 256;
        Z = (uint64_t *)malloc(zcap * sizeof(uint64_t));
        zc = 0;
    }

    while (zc < (size_t)n) {
        if (scanned < len) {
            if (W[scanned] == 0) {
                if (zc == zcap) {
                    zcap *= 2;
                    Z = (uint64_t *)realloc(Z, zcap * sizeof(uint64_t));
                }
                Z[zc++] = (uint64_t)scanned + 1;
            }
            scanned++;
        } else {
            /* unreachable in practice, scanned always trails; kept for safety */
            break;
        }
        if (scanned >= len) {
            /* expand: apply morphism to next unread letter */
            if (len + 4 > cap) {
                cap *= 2;
                W = (unsigned char *)realloc(W, cap);
            }
            if (W[rd] == 0) {
                W[len++] = 1;
            } else {
                W[len++] = 1; W[len++] = 0; W[len++] = 0; W[len++] = 1;
            }
            rd++;
        }
    }
    return (seq_t)Z[n - 1];
}
