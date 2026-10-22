/*
 * A367564  Triangular array read by rows: T(n, k) = binomial(n, k) *
 *          A001333(n - k).
 * https://oeis.org/A367564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r,k) * A001333(r-k) where r,k derived from n *
 *            via r = smallest integer with (r+1)*(r+2)/2 > n, * k = n -
 *            r*(r+1)/2. A001333(0)=1, A001333(1)=1,
 *            A001333(m)=2*A001333(m-1)+A001333(m-2)
 * Range:     u8 n <= 28, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"   /* provides binomial(idx_t, idx_t) */

seq_t a(idx_t n)
{
    /* find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    for (;; ++r) {
        /* compute (r+1)*(r+2)/2 safely */
        idx_t next = (r + 1ULL) * (r + 2ULL) / 2ULL;
        if (next > n)
            break;
    }
    idx_t start = r * (r + 1ULL) / 2ULL;
    idx_t k = n - start;          /* 0 <= k <= r */

    /* memoised A001333 values up to current r */
    static seq_t *a_cache = NULL;
    static size_t a_cap  = 0;      /* number of entries stored */

    if ((idx_t)r >= a_cap) {
        size_t new_cap = (size_t)r + 1;
        seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;           /* allocation failure, unlikely for given constraints */

        /* copy previous entries if any */
        if (a_cache)
            memcpy(tmp, a_cache, a_cap * sizeof(seq_t));

        /* compute new entries from a_cap to r */
        for (size_t i = a_cap; i <= (size_t)r; ++i) {
            if (i == 0)
                tmp[i] = (seq_t)1;
            else if (i == 1)
                tmp[i] = (seq_t)1;
            else
                tmp[i] = (seq_t)2 * tmp[i - 1] + tmp[i - 2];
        }

        if (a_cache)
            free(a_cache);
        a_cache = tmp;
        a_cap   = new_cap;
    }

    seq_t bin = binomial(r, k);
    seq_t val = bin * a_cache[r - k];
    return val;
}
