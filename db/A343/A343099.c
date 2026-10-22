/*
 * A343099  Sums of 3 distinct odd squares.
 * https://oeis.org/A343099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search/Generation: a(n) is the n‑th smallest number that can be written
   as the sum of three distinct odd squares. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* static storage for the cached sequence */
static bool      initialized = false;
static size_t    uniq_count   = 0;

static seq_t   *odd_sqs      = NULL;      /* squares of odd numbers */
static size_t   odd_cnt      = 0;
static seq_t   *sums         = NULL;      /* temporary storage for all sums */
static size_t   sums_sz      = 0;         /* capacity of sums[] */

/* one-time initializer; computes all needed odd squares and sums up to 80099
   (the 10000th term).  283 is the largest odd whose square participates in
   any such sum <= 80099, so we generate all odd numbers up to 283. */
static void init_seq(void)
{
    if (initialized) return;

    /* odd numbers from 1 to 283 inclusive, count = (283+1)/2 = 142 */
    odd_cnt = 142;
    odd_sqs = (seq_t *)malloc(odd_cnt * sizeof(seq_t));
    for (size_t i = 0; i < odd_cnt; ++i) {
        seq_t odd = (seq_t)(2 * (seq_t)i + 1);
        odd_sqs[i] = odd * odd;
    }

    /* number of distinct triples = C(odd_cnt,3) */
    sums_sz = (odd_cnt * (odd_cnt - 1) * (odd_cnt - 2)) / 6;
    sums = (seq_t *)malloc(sums_sz * sizeof(seq_t));

    size_t pos = 0;
    for (size_t a = 0; a + 2 < odd_cnt; ++a)
        for (size_t b = a + 1; b + 1 < odd_cnt; ++b)
            for (size_t c = b + 1; c < odd_cnt; ++c)
                sums[pos++] = odd_sqs[a] + odd_sqs[b] + odd_sqs[c];

    /* sort the sums */
    qsort(sums, pos, sizeof(seq_t), cmp_seq);

    /* deduplicate in place */
    size_t uniq = 0;
    for (size_t i = 0; i < pos; ++i) {
        if (i == 0 || sums[i] != sums[i - 1]) {
            sums[uniq++] = sums[i];
        }
    }
    uniq_count = uniq;

    initialized = true;
}

/* OEIS function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0 – return 0 */

    init_seq();

    size_t idx = (size_t)(n - 1);
    if (idx < uniq_count) return sums[idx];

    /* n is beyond precomputed terms */
    return 0;
}
