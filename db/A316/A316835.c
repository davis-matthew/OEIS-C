/*
 * A316835  Sums of four distinct positive even squares.
 * https://oeis.org/A316835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * (n-th smallest sum of four distinct squares of
 *            positive integers)
 * Range:     u8 none, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache of computed sums (not multiplied by 4). */
static seq_t *cache = NULL;
static size_t cached_len = 0;
static idx_t current_M = 0;

/* Comparator for qsort. */
static int cmp_seq_t(const void *p, const void *q) {
    seq_t a = *(const seq_t *)p;
    seq_t b = *(const seq_t *)q;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

/* Recompute cache to contain at least target_n distinct sums. */
static void recompute_cache(idx_t target_n) {
    idx_t M = current_M;
    if (M == 0) M = 10;

    while (1) {
        /* Estimate total number of combinations C(M,4).  Use 128-bit to avoid overflow. */
        unsigned __int128 t128 = (unsigned __int128)M * (M - 1) * (M - 2) * (M - 3);
        t128 /= 24;
        size_t total = (size_t)t128;  /* safe because M is small (≤ few hundred). */

        /* Allocate raw array for all sums, then compress unique values. */
        seq_t *raw = (seq_t *)malloc(total * sizeof(seq_t));
        if (!raw) { /* allocation failure – abort. */
            exit(1);
        }

        size_t idx = 0;
        for (idx_t i = 1; i <= M - 3; ++i) {
            seq_t sq_i = (seq_t)i * (seq_t)i;
            for (idx_t j = i + 1; j <= M - 2; ++j) {
                seq_t sq_j = (seq_t)j * (seq_t)j;
                seq_t sum_ij = sq_i + sq_j;
                for (idx_t k = j + 1; k <= M - 1; ++k) {
                    seq_t sq_k = (seq_t)k * (seq_t)k;
                    seq_t sum_ijk = sum_ij + sq_k;
                    for (idx_t l = k + 1; l <= M; ++l) {
                        seq_t sq_l = (seq_t)l * (seq_t)l;
                        raw[idx++] = sum_ijk + sq_l;
                    }
                }
            }
        }

        /* Sort all sums. */
        qsort(raw, idx, sizeof(seq_t), cmp_seq_t);

        /* Keep only unique values.  We can compress in place. */
        size_t uniq = 0;
        for (size_t i = 0; i < idx; ++i) {
            if (uniq == 0 || raw[i] != raw[uniq - 1]) {
                raw[uniq++] = raw[i];
            }
        }

        if (uniq < target_n) {
            /* Need more numbers – increase M and retry. */
            free(raw);
            M += M / 2 + 10;  /* increase roughly by 50% + 10 to converge quickly */
            continue;
        }

        /* Replace old cache. */
        free(cache);
        cache = raw;
        cached_len = uniq;
        current_M = M;
        return;
    }
}

seq_t a(idx_t n) {
    if (n == 0)  /* undefined for offset 1; return 0 as placeholder. */
        return 0;

    if (n > cached_len) {
        recompute_cache(n);
    }
    return cache[n - 1] * ((seq_t)4);
}
