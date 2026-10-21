/*
 * A261877  Expansion of psi(x^4) / phi(-x^3) in powers of x where phi(),
 *          psi() are Ramanujan theta functions.
 * https://oeis.org/A261877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Expansion of psi(x^4)/phi(-x^3) in powers of x (OEIS A261877)
 * Range:     u8 n <= 29, u16 n <= 80, u32 n <= 239, u64 n <= 797,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static const int period[24] = {
    0, 0, 2, 1, 0, 1, 0, -1, 2, 0, 0, 2,
    0, 0, 2, -1, 0, 1, 0, 1, 2, 0, 0, 0
};

seq_t a(idx_t n)
{
    idx_t N = n;
    size_t len = (size_t)N + 1;

#if defined(__cplusplus)
    seq_t *cur  = new seq_t[len];
    seq_t *nxt  = new seq_t[len];
    seq_t *tmp  = new seq_t[len];
#else
    seq_t *cur  = (seq_t*)malloc(len * sizeof(seq_t));
    seq_t *nxt  = (seq_t*)malloc(len * sizeof(seq_t));
    seq_t *tmp  = (seq_t*)malloc(len * sizeof(seq_t));
#endif

    /* initialize */
    for (size_t i = 0; i < len; ++i) cur[i] = 0;
    cur[0] = 1;

    for (idx_t k = 1; k <= N; ++k) {
        int e = period[(k-1) % 24];
        if (e == 0) continue;

        if (e == 1) {
            nxt[0] = cur[0];
            for (size_t i = 1; i < len; ++i) {
                nxt[i] = cur[i];
                if (i >= (size_t)k) nxt[i] += nxt[i - (size_t)k];
            }
        } else if (e == 2) {
            /* s = nxt (intermediate) */
            nxt[0] = cur[0];
            for (size_t i = 1; i < len; ++i) {
                nxt[i] = cur[i];
                if (i >= (size_t)k) nxt[i] += nxt[i - (size_t)k];
            }
            /* t in tmp */
            tmp[0] = 0;
            for (size_t i = 1; i < len; ++i) {
                if (i < (size_t)k) tmp[i] = 0;
                else tmp[i] = tmp[i - (size_t)k] + nxt[i] - cur[i];
            }
            /* final sum */
            for (size_t i = 0; i < len; ++i) nxt[i] += tmp[i];
        } else if (e == -1) {
            for (size_t i = 0; i < len; ++i) nxt[i] = cur[i];
            for (size_t i = (size_t)k; i < len; ++i) nxt[i] -= cur[i - (size_t)k];
        }

        /* swap current and next */
        seq_t *tmpptr = cur;
        cur = nxt;
        nxt = tmpptr;
    }

    seq_t result = cur[(size_t)N];

#if defined(__cplusplus)
    delete[] cur;
    delete[] nxt;
    delete[] tmp;
#else
    free(cur);
    free(nxt);
    free(tmp);
#endif

    return result;
}
