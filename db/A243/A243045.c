/*
 * A243045  Largest number k such that k*n/(k+n) and k*n/(k-n) are integers or
 *          0 if no such number exists.
 * https://oeis.org/A243045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 1299, u32 n <= 9763, u64 n <= 9763,
 *            u128 n <= 9763, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Largest k such that k*n/(k+n) and k*n/(k-n) are integers.  Sequence A243045. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Return the divisors of n in *divs.  The array is malloc'd and must be free'd by the caller. */
static idx_t get_divisors(idx_t n, idx_t **divs) {
    idx_t *tmp = (idx_t *)malloc(sizeof(idx_t) * 16);
    if (!tmp) { *divs = NULL; return 0; }
    size_t cap = 16, cnt = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            if (cnt == cap) { cap <<= 1; tmp = (idx_t *)realloc(tmp, sizeof(idx_t) * cap); }
            tmp[cnt++] = i;
            idx_t q = n / i;
            if (q != i) {
                if (cnt == cap) { cap <<= 1; tmp = (idx_t *)realloc(tmp, sizeof(idx_t) * cap); }
                tmp[cnt++] = q;
            }
        }
    }
    *divs = tmp;
    return (idx_t)cnt;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;

    idx_t *divs = NULL;
    idx_t dcnt = get_divisors(n, &divs);
    if (!divs) return 0;

    seq_t best = 0;
    for (idx_t i = 0; i < dcnt; ++i) {
        idx_t d1 = divs[i];
        for (idx_t j = 0; j < dcnt; ++j) {
            idx_t d2 = divs[j];
            if (d1 <= d2) continue;          /* need d1 > d2 */
            if ((d1 & 1) != (d2 & 1)) continue; /* same parity needed */

            idx_t u = (d1 + d2) >> 1;        /* (d1+d2)/2 */
            idx_t v = (d1 - d2) >> 1;        /* (d1-d2)/2 */
            if (v == 0) continue;            /* avoid division by zero */

            seq_t prod = (seq_t)n * (seq_t)u;
            if (prod % v != 0) continue;     /* k must be integer */

            seq_t k = prod / v;
            if (k <= (seq_t)n) continue;     /* k must be > n to avoid division by zero */

            /* Verify the two divisibility conditions (safety check) */
            if ((k * (seq_t)n) % (k + (seq_t)n) != 0) continue;
            if ((k * (seq_t)n) % (k - (seq_t)n) != 0) continue;

            if (k > best) best = k;
        }
    }
    free(divs);
    return best;
}
