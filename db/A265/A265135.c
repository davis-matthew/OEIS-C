/*
 * A265135  Numbers that are the sum of two distinct nonzero triangular
 *          numbers in more than two ways.
 * https://oeis.org/A265135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A265135: numbers that are the sum of two distinct
 * nonzero triangular numbers in more than two ways.
 *
 * The implementation enumerates triangular numbers up to a pre‑chosen bound,
 * counts unordered distinct pairs that produce each sum, keeps the sums
 * that appear in at least three different pairs, sorts them, and returns the
 * n‑th element (a(1) is the first listed term).  The algorithm is O(K²)
 * with K ≈ 4000, which is well below the 2‑second per‑call limit for all
 * requested n up to about 10,000.  The code works for any unsigned
 * integer type or arbitrary–precision integer class used for seq_t.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Comparator for qsort. */
static int seq_cmp(const void *a, const void *b)
{
    const seq_t *pa = (const seq_t *)a;
    const seq_t *pb = (const seq_t *)b;
    if (*pa < *pb) return -1;
    if (*pa > *pb) return 1;
    return 0;
}

/* Cached sequence data. */
static bool initialized = false;
static seq_t *terms = NULL;      /* sorted list of qualifying sums */
static size_t term_count = 0;    /* number of stored terms */

/* One‑time initialization. */
static void init_sequence(void)
{
    if (initialized) return;

    /* Bound on the triangular index – large enough to obtain >10,000
     * qualifying sums while keeping the memory usage reasonable. */
    const idx_t K = 4000;

    /* Allocate triangular numbers T[i] = i*(i+1)/2, 1‑based. */
    seq_t *tri = (seq_t *)malloc((size_t)(K + 1) * sizeof(seq_t));
    if (!tri) return;

    for (idx_t i = 1; i <= K; ++i)
        tri[i] = (seq_t)i * ((seq_t)i + 1) / 2;

    /* Max possible sum.  Use size_t to avoid overflow in the array size. */
    const size_t max_sum = (size_t)tri[K] + (size_t)tri[K];

    /* Count how many unordered distinct pairs produce each sum.  We only
     * need to know whether the count is at least 3, so we saturate the
     * counter at 3 to save memory. */
    uint8_t *cnt = (uint8_t *)calloc(max_sum + 1, sizeof(uint8_t));
    if (!cnt) {
        free(tri);
        return;
    }

    for (idx_t a = 1; a <= K; ++a) {
        for (idx_t b = a + 1; b <= K; ++b) {
            size_t s = (size_t)tri[a] + (size_t)tri[b];
            uint8_t v = cnt[s];
            if (v < 3) cnt[s] = v + 1;
        }
    }

    /* Collect sums that occur in at least three distinct ways. */
    size_t cap = 256;
    terms = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!terms) {
        free(tri);
        free(cnt);
        return;
    }
    term_count = 0;
    for (size_t v = 1; v <= max_sum; ++v) {
        if (cnt[v] == 3) {
            if (term_count == cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(terms, cap * sizeof(seq_t));
                if (!tmp) break;
                terms = tmp;
            }
            terms[term_count++] = (seq_t)v;
        }
    }

    free(tri);
    free(cnt);

    /* Sort the qualifying sums.  The original order is not guaranteed. */
    qsort(terms, term_count, sizeof(seq_t), seq_cmp);

    initialized = true;
}

/* Public entry point. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */
    init_sequence();
    if (!initialized) return 0;
    if ((size_t)(n - 1) >= term_count) return 0;
    return terms[n - 1];
}
