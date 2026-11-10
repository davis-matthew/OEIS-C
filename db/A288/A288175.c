/*
 * A288175  Positions of 1 in A288173; complement of A288174.
 * https://oeis.org/A288175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A288173, the fixed point of
 *            00->0010, 1->001 starting from 00 (computed by generating the
 *            word)
 * Range:     u8 n <= 69, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9538 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static unsigned char *w = NULL;
    static size_t len = 0;
    static size_t *pos = NULL;
    static size_t npos = 0;

    if (w == NULL) {
        w = (unsigned char *)malloc(2);
        w[0] = 0; w[1] = 0;
        len = 2;
        pos = NULL;
        npos = 0;
    }

    while (npos < (size_t)n) {
        size_t nl = 0, i = 0, j = 0;
        while (i < len) {
            if (w[i] == 0 && i + 1 < len && w[i + 1] == 0) { nl += 4; i += 2; }
            else if (w[i] == 1) { nl += 3; i += 1; }
            else { nl += 1; i += 1; }
        }
        unsigned char *nx = (unsigned char *)malloc(nl);
        i = 0;
        while (i < len) {
            if (w[i] == 0 && i + 1 < len && w[i + 1] == 0) {
                nx[j++] = 0; nx[j++] = 0; nx[j++] = 1; nx[j++] = 0; i += 2;
            } else if (w[i] == 1) {
                nx[j++] = 0; nx[j++] = 0; nx[j++] = 1; i += 1;
            } else {
                nx[j++] = 0; i += 1;
            }
        }
        free(w);
        w = nx;
        len = nl;

        size_t cnt = 0;
        for (i = 0; i < len; i++) if (w[i] == 1) cnt++;
        free(pos);
        pos = (size_t *)malloc((cnt + 1) * sizeof(size_t));
        npos = 0;
        for (i = 0; i < len; i++) if (w[i] == 1) pos[npos++] = i + 1;
    }

    return (seq_t)pos[(size_t)n - 1];
}
