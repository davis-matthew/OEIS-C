/*
 * A339509  Number of subsets of {1..n} whose elements have the same greatest
 *          prime factor.
 * https://oeis.org/A339509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_p (2^{cnt_p} – 1), where cnt_p counts numbers in
 *            [1..n] whose greatest prime factor equals p (p = 1 for the
 *            number 1)
 * Range:     u8 n <= 34, u16 n <= 119, u32 n <= 377, u64 n <= 1322,
 *            u128 n <= 4159, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) {          /* only the empty set */
        return 1;
    }

    /* Allocate auxiliary arrays.  Size n+1 fits into size_t. */
    size_t sz = (size_t)(n + 1);
    idx_t  *spf  = (idx_t *)malloc(sz * sizeof(idx_t));
    idx_t  *gpf  = (idx_t *)malloc(sz * sizeof(idx_t));
    size_t *cnt  = (size_t *)calloc(sz, sizeof(size_t));
    if (!spf || !gpf || !cnt) {
        free(spf); free(gpf); free(cnt);
        return 0;               /* allocation failure */
    }

    /* Initialize smallest prime factor array. */
    for (idx_t i = 0; i <= n; ++i) {
        spf[i] = i;
    }
    spf[0] = 0;
    spf[1] = 1;

    /* Sieve for smallest prime factor. */
    for (idx_t i = 2; i * i <= n; ++i) {
        if (spf[i] == i) {          /* i is prime */
            for (idx_t j = i * i; j <= n; j += i) {
                if (spf[j] == j) {
                    spf[j] = i;
                }
            }
        }
    }

    /* Compute greatest prime factor for every integer. */
    gpf[0] = 0;
    gpf[1] = 1;
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == i) {
            gpf[i] = i;
        } else {
            idx_t r  = i / spf[i];
            idx_t mx = gpf[r];
            gpf[i] = (spf[i] > mx) ? spf[i] : mx;
        }
    }

    /* Count members of each GPF group.  Include group 1 (the number 1). */
    cnt[1] = 1;                    /* number 1 */
    for (idx_t i = 2; i <= n; ++i) {
        ++cnt[gpf[i]];
    }

    /* Compute the result. */
    seq_t res = 1;                 /* the empty set */

    for (idx_t p = 1; p <= n; ++p) {
        size_t c = cnt[p];
        if (c == 0) {
            continue;
        }
        /* Compute 2^c. */
        seq_t pow2 = 1;
        for (size_t k = 0; k < c; ++k) {
            pow2 <<= 1;            /* multiply by 2 */
        }
        res += pow2 - 1;          /* exclude the empty subset of this group */
    }

    /* Free allocated memory. */
    free(spf);
    free(gpf);
    free(cnt);

    return res;
}
