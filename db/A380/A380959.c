/*
 * A380959  Array read by antidiagonals downward where A(n,k) is the number of
 *          integer partitions of k with product n.
 * https://oeis.org/A380959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of integer partitions of sum_k(n) with product
 *            p(n), where (p(n),sum_k(n)) is obtained by reading the array
 *            A(n,k) along antidiagonals downward with offset 0
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   _build_exact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/_build_exact.h"

static seq_t **_cum_cache = NULL;     /* _cum_cache[p] points to array of size p+1 */
static size_t _cum_len   = 0;         /* number of entries stored (max product+1) */

seq_t a(idx_t n)
{
    /* Antidiagonal mapping: product = offset+1, sum = diagonal - offset */
    if (n == 0) return 1;

    /* Find diagonal d with d*(d+1)/2 ≤ n < (d+1)*(d+2)/2 */
    idx_t d = 0;
    while (( (__int128)(d + 1) * (d + 2) ) / 2 <= n) {
        ++d;
    }

    idx_t t      = ( (__int128)d * (d + 1) ) / 2;
    idx_t offset = n - t;
    idx_t product = offset + 1;
    idx_t sumk    = d - offset;   /* sum (k) */

    if (product == 1) {           /* all-ones partitions */
        return 1;
    }

    /* Allocate cache slot for this product if necessary */
    if ((size_t)product >= _cum_len) {
        size_t new_len = product + 1;
        seq_t **tmp = (seq_t**)realloc(_cum_cache, new_len * sizeof(seq_t*));
        if (!tmp) return 0;       /* allocation failure */
        for (size_t i = _cum_len; i < new_len; ++i) tmp[i] = NULL;
        _cum_cache = tmp;
        _cum_len   = new_len;
    }

    /* Build exact and cumulative arrays for this product on first use */
    if (!_cum_cache[product]) {
        seq_t *exact = (seq_t*)calloc(product + 1, sizeof(seq_t));
        if (!exact) return 0;
        _build_exact(product, 2, 0, exact);

        seq_t *cum = (seq_t*)malloc((product + 1) * sizeof(seq_t));
        if (!cum) { free(exact); return 0; }
        cum[0] = exact[0];
        for (idx_t i = 1; i <= product; ++i) {
            cum[i] = cum[i-1] + exact[i];
        }
        free(exact);
        _cum_cache[product] = cum;
    }

    /* Answer is number of factorizations with sum ≤ sumk */
    if (sumk >= product) {
        return _cum_cache[product][product];
    } else {
        return _cum_cache[product][sumk];
    }
}
