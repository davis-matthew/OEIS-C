/*
 * A289966  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 625", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A289966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0..n} c(n; k,k) * 2^(n-k), where c(t;x,y) is the
 *            2D outer-totalistic CA "Rule 625" (5-cell von Neumann, new state
 *            = bit (2*(sum of 4 neighbours)+centre) of 625) started from one
 *            ON cell; computed by direct simulation
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 117 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    int N = (int)n;
    int M = 2 * N + 3;
    int c0 = N + 1;
    unsigned char *cur = (unsigned char *)calloc((size_t)M * M, 1);
    unsigned char *nxt = (unsigned char *)calloc((size_t)M * M, 1);
    int bg = 0;
    cur[c0 * M + c0] = 1;
    for (int t = 0; t < N; t++) {
        int nbg = (625 >> (2 * 4 * bg + bg)) & 1;
        for (int y = 0; y < M; y++) {
            for (int x = 0; x < M; x++) {
                if (x == 0 || y == 0 || x == M - 1 || y == M - 1) {
                    nxt[y * M + x] = (unsigned char)nbg;
                    continue;
                }
                int s = cur[(y - 1) * M + x] + cur[(y + 1) * M + x]
                      + cur[y * M + x - 1] + cur[y * M + x + 1];
                int idx = 2 * s + cur[y * M + x];
                nxt[y * M + x] = (unsigned char)((625 >> idx) & 1);
            }
        }
        unsigned char *tmp = cur; cur = nxt; nxt = tmp;
        bg = nbg;
    }
    seq_t v = 0;
    for (int k = 0; k <= N; k++) {
        v = v * 2 + (seq_t)cur[(c0 + k) * M + (c0 + k)];
    }
    free(cur);
    free(nxt);
    return v;
}
