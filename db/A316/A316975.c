/*
 * A316975  a(n) is the minimum number of occurrences of the same value
 *          (r0-r1+n) mod n for all pairs (r0,r1) of quadratic residues mod n.
 * https://oeis.org/A316975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = minimum positive occurrence count of (r0 - r1 + n) mod n
 *            * over all ordered pairs of quadratic residues r0,r1 modulo n
 * Range:     u8 n <= 718, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Step 1: mark all quadratic residues modulo n. */
    bool *isQR = (bool *)calloc((size_t)n, sizeof(bool));
    if (!isQR) exit(EXIT_FAILURE);
    for (idx_t x = 0; x < n; ++x) {
        idx_t r = (x * x) % n;
        isQR[r] = true;
    }

    /* Step 2: build a compact list of the residues. */
    idx_t *res = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!res) { free(isQR); exit(EXIT_FAILURE); }
    size_t res_len = 0;
    for (idx_t i = 0; i < n; ++i) {
        if (isQR[i]) {
            res[res_len++] = i;
        }
    }

    /* Step 3: count how many times each difference occurs. */
    seq_t *cnt = (seq_t *)calloc((size_t)n, sizeof(seq_t));
    if (!cnt) { free(isQR); free(res); exit(EXIT_FAILURE); }

    for (size_t i = 0; i < res_len; ++i) {
        idx_t r0 = res[i];
        for (size_t j = 0; j < res_len; ++j) {
            idx_t r1   = res[j];
            idx_t diff = (r0 + n - r1) % n;
            cnt[diff] += (seq_t)1;
        }
    }

    /* Step 4: find the smallest positive count. */
    seq_t mincnt = 0;                /* 0 will mean "not set yet" */
    for (idx_t i = 0; i < n; ++i) {
        seq_t c = cnt[i];
        if (c > 0 && (mincnt == 0 || c < mincnt)) {
            mincnt = c;
        }
    }

    /* Clean up allocated memory. */
    free(isQR);
    free(res);
    free(cnt);

    return mincnt;
}
