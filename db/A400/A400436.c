/*
 * A400436  Integers that can be expressed as the sum of 5 factorials.
 * https://oeis.org/A400436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 3153, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A400436: Integers that can be expressed as a sum of 5 factorials.
   The function builds the sorted list of all distinct sums of
   5 factorials 0! … 20! the first time it is called, then returns the
   n‑th term (a(1) = 5, a(2) = 6, …).  For larger seq_t sizes the
   same list is used, and out‑of‑range indices return 0. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#ifdef __cplusplus
#include <algorithm>
#endif

/* ---------- helpers for sorting (C only) ---------- */
static void swap_seq(seq_t *a, seq_t *b) {
    seq_t tmp = *a; *a = *b; *b = tmp;
}

static int cmp_seq(const seq_t *a, const seq_t *b) {
    return (*a < *b) ? -1 : ((*a > *b) ? 1 : 0);
}

static void quicksort_seq(seq_t *arr, int left, int right) {
    if (left >= right) return;
    seq_t pivot = arr[(left + right) / 2];
    int i = left, j = right;
    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;
        if (i <= j) {
            swap_seq(&arr[i], &arr[j]);
            i++; j--;
        }
    }
    quicksort_seq(arr, left, j);
    quicksort_seq(arr, i, right);
}

/* ---------- static state ---------- */
static seq_t *sums_unique = NULL; /* sorted unique sums */
static size_t sums_count  = 0;
static bool   initialized = false;

/* ---------- binomial coefficient for small n,k (n <= 30) ---------- */
static inline size_t binomial_cnk(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k > n - k) k = n - k;
    size_t res = 1;
    for (int i = 1; i <= k; ++i)
        res = res * (size_t)(n - k + i) / (size_t)i;
    return res;
}

/* ---------- initialise the sequence ---------- */
static void init_sequence(void) {
    if (initialized) return;
    initialized = true;

    /* factorial limit – 20! fits within 64‑bit and gives the
       first 10 000 terms of the sequence. */
    const int FACT_LIMIT = 20;
    const int TYPES = FACT_LIMIT + 1;                    /* 0! … FACT_LIMIT! */
    const size_t TOTAL = binomial_cnk(TYPES + 4, 5);     /* number of 5‑multisets */

    /* compute factorials 0! … FACT_LIMIT! */
    seq_t *fact = (seq_t *)malloc((FACT_LIMIT + 1) * sizeof(seq_t));
    if (!fact) return;
    fact[0] = (seq_t)1;
    for (int i = 1; i <= FACT_LIMIT; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* generate all sums of 5 factorials (with repetition). */
    seq_t *tmp = (seq_t *)malloc(TOTAL * sizeof(seq_t));
    if (!tmp) {
        free(fact); return;
    }
    size_t idx = 0;
    for (int i = 0; i <= FACT_LIMIT; ++i)
        for (int j = i; j <= FACT_LIMIT; ++j)
            for (int k = j; k <= FACT_LIMIT; ++k)
                for (int l = k; l <= FACT_LIMIT; ++l)
                    for (int m = l; m <= FACT_LIMIT; ++m)
                        tmp[idx++] = fact[i] + fact[j] + fact[k] + fact[l] + fact[m];

    /* sort the array of sums */
#ifdef __cplusplus
    std::sort(tmp, tmp + idx);
#else
    quicksort_seq(tmp, 0, (int)idx - 1);
#endif

    /* remove duplicates, store in a new array */
    if (idx == 0) {
        sums_unique = NULL;
        sums_count  = 0;
    } else {
        seq_t *unique = (seq_t *)malloc(idx * sizeof(seq_t));
        if (!unique) {
            free(tmp); free(fact); return;
        }
        size_t uniq_i = 0;
        unique[uniq_i++] = tmp[0];
        for (size_t s = 1; s < idx; ++s) {
            if (cmp_seq(&tmp[s], &tmp[s - 1]) != 0)
                unique[uniq_i++] = tmp[s];
        }
        seq_t *shrinked = (seq_t *)realloc(unique, uniq_i * sizeof(seq_t));
        sums_unique = shrinked ? shrinked : unique;
        sums_count  = uniq_i;
    }

    /* free temporary buffers */
    free(tmp);
    free(fact);
}

/* ---------- public function ---------- */
seq_t a(idx_t n) {
    init_sequence();
    if (n == 0 || (size_t)n > sums_count)
        return (seq_t)0; /* undefined index – safe fallback */
    return sums_unique[(size_t)n - 1];
}
