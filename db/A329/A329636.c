/*
 * A329636  Numbers that are sums of consecutive centered cube numbers
 *          (A005898).
 * https://oeis.org/A329636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = nth smallest sum of consecutive centered cube numbers
   (OEIS sequence A329636). The function builds a sorted list of all such
   sums up to a growing bound and returns the requested element. */

#include <stdlib.h>

static seq_t *sums = NULL;      /* sorted unique sums */
static size_t sums_len = 0;      /* number of stored sums */
static idx_t current_M = 0;     /* largest index of centered cubes processed */

/* Comparator for qsort on seq_t values */
static int cmp_seq_t(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Ensure that at least 'need' sums are available in the cache. */
static void ensure_cache(size_t need)
{
    while (sums_len < need) {
        /* Grow the range of centered cubes. */
        idx_t newM = (current_M == 0 ? 200 : current_M + 200);
        current_M = newM;

        /* Prefix sums of centered cube numbers: c(k) = 2k^3 - 3k^2 + 3k - 1 */
        seq_t *prefix = (seq_t *)malloc(sizeof(seq_t) * (size_t)(current_M + 1));
        if (!prefix) abort();
        prefix[0] = 0;
        for (idx_t i = 1; i <= current_M; ++i) {
            seq_t k = (seq_t)i;
            seq_t c_val = 2 * k * k * k - 3 * k * k + 3 * k - 1;
            prefix[i] = prefix[i - 1] + c_val;
        }

        /* Generate all sums of consecutive centered cubes. */
        size_t tot_pairs = (size_t)current_M * (current_M + 1) / 2;
        seq_t *all_sums = (seq_t *)malloc(sizeof(seq_t) * tot_pairs);
        if (!all_sums) abort();
        size_t pos = 0;
        for (idx_t end = 1; end <= current_M; ++end) {
            for (idx_t start = 0; start < end; ++start) {
                all_sums[pos++] = prefix[end] - prefix[start];
            }
        }

        /* Sort the sums. */
        qsort(all_sums, tot_pairs, sizeof(seq_t), cmp_seq_t);

        /* Count unique values. */
        size_t uniq_cnt = 0;
        if (tot_pairs > 0) {
            uniq_cnt = 1;
            seq_t last = all_sums[0];
            for (size_t k = 1; k < tot_pairs; ++k) {
                if (all_sums[k] != last) {
                    last = all_sums[k];
                    ++uniq_cnt;
                }
            }
        }

        /* Allocate array of unique sums. */
        seq_t *uniq = NULL;
        if (uniq_cnt > 0) {
            uniq = (seq_t *)malloc(sizeof(seq_t) * uniq_cnt);
            if (!uniq) abort();
            uniq[0] = all_sums[0];
            size_t idx = 1;
            seq_t last = all_sums[0];
            for (size_t k = 1; k < tot_pairs; ++k) {
                if (all_sums[k] != last) {
                    last = all_sums[k];
                    uniq[idx++] = last;
                }
            }
        }

        /* Replace old cache with new one. */
        free(prefix);
        free(all_sums);
        free(sums);
        sums = uniq;
        sums_len = uniq_cnt;
    }
}

/* Main function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out of range: return 0 */
    ensure_cache((size_t)n);
    return sums[n - 1];
}
