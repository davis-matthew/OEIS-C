/*
 * A319185  Numbers that are sums of consecutive hexagonal numbers (A000384).
 * https://oeis.org/A319185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A319185: Numbers that are sums of consecutive hexagonal numbers.
   Computation method: generate all sums of consecutive hexagonal numbers
   up to a fixed bound, sort them, deduplicate, and cache the resulting
   sorted list.  a(1)=0. */
#include <stdlib.h>
#include <stdbool.h>

/* Comparator for qsort */
static int seq_cmp(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Cached sequence */
static seq_t *terms     = NULL;
static size_t terms_cnt = 0;

/* Build the sequence lazily */
static void build_sequence(void)
{
    /* Bound on the number of hexagonal numbers: enough to exceed 10 000
       distinct sums (N(N+1)/2 ≈ 2 001 000 uniq for N=2000). */
    const idx_t N = 2000;

    /* Allocate arrays for hexagonal numbers and their prefix sums. */
    seq_t *hex  = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *pref = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!hex || !pref) {
        free(hex); free(pref);
        return;
    }

    hex[0]  = 0;
    pref[0] = 0;
    for (idx_t k = 1; k <= N; ++k) {
        seq_t h = (seq_t)k * ((seq_t)2 * k - 1);   /* H_k = k(2k-1) */
        hex[k]  = h;
        pref[k] = pref[k - 1] + h;
    }

    /* Number of interval sums plus the empty sum (0). */
    size_t total = (size_t)N * (N + 1) / 2 + 1;

    seq_t *all = (seq_t *)malloc(total * sizeof(seq_t));
    if (!all) {
        free(hex); free(pref);
        return;
    }

    /* Generate all consecutive sums. */
    size_t pos = 0;
    all[pos++] = 0;                       /* empty sum */
    for (idx_t i = 1; i <= N; ++i) {
        for (idx_t j = i; j <= N; ++j) {
            all[pos++] = pref[j] - pref[i - 1];
        }
    }

    /* Sort and remove duplicates. */
    qsort(all, total, sizeof(seq_t), seq_cmp);

    /* Allocate the unique list. */
    terms = (seq_t *)malloc(total * sizeof(seq_t));
    if (!terms) {
        free(all); free(hex); free(pref);
        return;
    }

    size_t uniq = 0;
    for (size_t i = 0; i < total; ++i) {
        if (i == 0 || all[i] != all[i - 1]) {
            terms[uniq++] = all[i];
        }
    }
    terms_cnt = uniq;

    free(all);
    free(hex);
    free(pref);
}

/* Return the n‑th term of the sequence (a(1) = 0). */
seq_t a(idx_t n)
{
    if (!terms) build_sequence();
    if (n == 0 || n > terms_cnt) return 0;   /* out of range fallback */
    return terms[n - 1];
}
