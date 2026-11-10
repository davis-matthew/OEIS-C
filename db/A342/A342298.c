/*
 * A342298  a(n) is the greatest k such that A342263(k) = n.
 * https://oeis.org/A342298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = largest binary string of length 2^(n+1)+n whose
 *            (n+1)-windows are all distinct (lexicographically largest linear
 *            de Bruijn sequence of order n+1), found by DFS preferring 1;
 *            matches A070939(a(n)) = A052944(n+1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 7 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    unsigned k = (unsigned)n + 1;
    size_t W = (size_t)1 << k;
    size_t L = W + k - 1;
    size_t mask = W - 1;

    unsigned char *b = (unsigned char *)calloc(L, 1);
    unsigned char *state = (unsigned char *)calloc(L + 1, 1);
    unsigned char *seen = (unsigned char *)calloc(W, 1);
    size_t *w = (size_t *)calloc(L, sizeof(size_t));

    for (size_t i = 0; i < k; ++i) b[i] = 1;
    w[k - 1] = mask;
    seen[mask] = 1;

    size_t pos = k;
    while (pos < L) {
        unsigned opt = state[pos]++;
        if (opt >= 2) {
            state[pos] = 0;
            --pos;
            seen[w[pos]] = 0;
            continue;
        }
        unsigned bit = (opt == 0) ? 1u : 0u;
        size_t nw = ((w[pos - 1] << 1) | bit) & mask;
        if (!seen[nw]) {
            seen[nw] = 1;
            b[pos] = (unsigned char)bit;
            w[pos] = nw;
            ++pos;
        }
    }

    seq_t res = 0;
    for (size_t i = 0; i < L; ++i) {
        res = res * 2 + (seq_t)b[i];
    }

    free(b);
    free(state);
    free(seen);
    free(w);
    return res;
}
