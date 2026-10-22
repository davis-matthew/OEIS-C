/*
 * A344642  Numbers that are the sum of four fifth powers in exactly one way.
 * https://oeis.org/A344642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 316, u32 n <= 9168, u64 n <= 9168,
 *            u128 n <= 9168, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that are the sum of four fifth powers in exactly one way,
   computed by brute‑force enumeration of non‑decreasing quadruples
   (i ≤ j ≤ k ≤ l). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* comparison routine for qsort; works for any sized unsigned seq_t */
static int cmp_seq_t(const void *a, const void *b) {
    const seq_t *va = (const seq_t *)a;
    const seq_t *vb = (const seq_t *)b;
    if (*va < *vb) return -1;
    if (*va > *vb) return 1;
    return 0;
}

/* static storage for the generated sequence */
static seq_t *term_array = NULL;
static size_t term_count = 0;
static bool initialized = false;

/* generate terms up to a sufficiently high bound, then store them in
   term_array in ascending order.  Maximum exponent is chosen so that
   at least 20000 terms are produced (far more than the test range). */
static void compute_terms(void) {
    /* maximum integer whose fifth power we will consider */
    const int MAXA = 80;  /* 80^5 = 3,276,800,000 */

    /* pre‑compute the fifth powers in 64‑bit to avoid overflow before
       casting to seq_t later (seq_t may be 8 or 16 bits). */
    uint64_t powv[MAXA + 1];
    for (int i = 0; i <= MAXA; ++i) {
        powv[i] = (uint64_t)i * i * i * i * i;
    }

    /* count how many valid quadruples exist (all sums ≤ max seq_t) */
    size_t total = 0;
    for (int a = 1; a <= MAXA; ++a)
        for (int b = a; b <= MAXA; ++b)
            for (int c = b; c <= MAXA; ++c)
                for (int d = c; d <= MAXA; ++d)
                    ++total;

    /* allocate temporary array for all sums that fit into seq_t */
    seq_t *sums = (seq_t *)malloc(total * sizeof(seq_t));
    size_t pos = 0;

    /* maximum representable value of seq_t (converted to 64‑bit) */
    uint64_t limit64 = (sizeof(seq_t) <= 8) ? (uint64_t)(seq_t)-1 : UINT64_MAX;

    for (int a = 1; a <= MAXA; ++a)
        for (int b = a; b <= MAXA; ++b)
            for (int c = b; c <= MAXA; ++c)
                for (int d = c; d <= MAXA; ++d) {
                    uint64_t sum = powv[a] + powv[b] + powv[c] + powv[d];
                    if (sum <= limit64) {
                        sums[pos++] = (seq_t)sum;
                    }
                }

    /* resize for actual number of sums retained */
    total = pos;
    /* sort the sums to identify duplicate representations */
    qsort(sums, total, sizeof(seq_t), cmp_seq_t);

    /* first count how many sums occur exactly once */
    size_t singles = 0;
    for (size_t i = 0; i < total; ) {
        size_t j = i + 1;
        while (j < total && sums[j] == sums[i]) ++j;
        if (j == i + 1) ++singles;
        i = j;
    }

    /* store the unique sums in term_array */
    term_array = (seq_t *)malloc(singles * sizeof(seq_t));
    size_t idx = 0;
    for (size_t i = 0; i < total; ) {
        size_t j = i + 1;
        while (j < total && sums[j] == sums[i]) ++j;
        if (j == i + 1) term_array[idx++] = sums[i];
        i = j;
    }
    term_count = singles;
    free(sums);
    initialized = true;
}

/* ------------------------------------------------------------------ */
/*  External function required by the build.                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized) compute_terms();          /* generate once           */
    if (n == 0) return 0;                       /* offset is 1, below not used */
    if ((idx_t)n > term_count) return 0;         /* safe fallback            */
    return term_array[(size_t)n - 1];
}
