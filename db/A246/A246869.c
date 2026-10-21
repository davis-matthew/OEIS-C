/*
 * A246869  Cube root of the smallest of the largest absolute values of parts
 *          of the partitions of n into four cubes, or -1 if no such partition
 *          exists.
 * https://oeis.org/A246869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal maximum absolute part (base of the largest cube)
 *            in a four‑cube partition of n. If no such partition exists, a(n)
 *            = -1
 * Range:     u8 n <= 5169, u16 n <= 8301, u32 n <= 8301, u64 n <= 8301,
 *            u128 n <= 8301, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_PART 500                    /* |base| <= 500 is safe for n <= 10000 */

struct pair {
    int64_t sum;
    int     maxabs;
};

static struct pair *pair_table = NULL;
static size_t pair_table_size = 0;

static int64_t *uniq_sums = NULL;
static int     *uniq_maxabs = NULL;
static size_t uniq_size = 0;
static bool initialized = false;

/* Comparator for qsort */
static int pair_cmp(const void *a, const void *b)
{
    const struct pair *pa = (const struct pair *)a;
    const struct pair *pb = (const struct pair *)b;
    if (pa->sum < pb->sum) return -1;
    if (pa->sum > pb->sum) return 1;
    return 0;
}

/* Precompute all unique sums of two cubes and minimal max(|a|,|b|) for each sum. */
static void init(void)
{
    if (initialized) return;
    const int limit = MAX_PART;
    const size_t num_pairs = (size_t)(2 * limit + 1) * (size_t)(2 * limit + 1);

    pair_table = (struct pair *)malloc(num_pairs * sizeof(struct pair));
    if (!pair_table) exit(1);  /* allocation failure */

    size_t idx = 0;
    for (int a = -limit; a <= limit; ++a) {
        int64_t ca = (int64_t)a * a * a;
        int abs_a = a >= 0 ? a : -a;
        for (int b = -limit; b <= limit; ++b) {
            int64_t cb = (int64_t)b * b * b;
            int abs_b = b >= 0 ? b : -b;
            pair_table[idx].sum = ca + cb;
            pair_table[idx].maxabs = abs_a > abs_b ? abs_a : abs_b;
            ++idx;
        }
    }

    qsort(pair_table, num_pairs, sizeof(struct pair), pair_cmp);

    uniq_sums = (int64_t *)malloc(num_pairs * sizeof(int64_t));
    uniq_maxabs = (int *)malloc(num_pairs * sizeof(int));
    if (!uniq_sums || !uniq_maxabs) exit(1);

    size_t write = 0;
    size_t i = 0;
    while (i < num_pairs) {
        int64_t cur_sum = pair_table[i].sum;
        int best = pair_table[i].maxabs;
        ++i;
        while (i < num_pairs && pair_table[i].sum == cur_sum) {
            if (pair_table[i].maxabs < best) best = pair_table[i].maxabs;
            ++i;
        }
        uniq_sums[write] = cur_sum;
        uniq_maxabs[write] = best;
        ++write;
    }
    uniq_size = write;

    free(pair_table);
    pair_table = NULL;

    initialized = true;
}

/* Binary search for value 'need' in uniq_sums, returns index or uniq_size if not found. */
static size_t find_index(int64_t need)
{
    size_t lo = 0, hi = uniq_size;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (uniq_sums[mid] < need)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    init();
    int best = INT_MAX;

    for (size_t i = 0; i < uniq_size; ++i) {
        int64_t need = (int64_t)n - uniq_sums[i];
        size_t j = find_index(need);
        if (j < uniq_size && uniq_sums[j] == need) {
            int cand = uniq_maxabs[i] > uniq_maxabs[j] ? uniq_maxabs[i] : uniq_maxabs[j];
            if (cand < best) best = cand;
            if (best == 0) break;  /* cannot improve further */
        }
    }

    if (best == INT_MAX) return (seq_t)-1;  /* no representation */
    return (seq_t)best;
}
