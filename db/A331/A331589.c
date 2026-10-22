/*
 * A331589  a(n) is the greatest k such that A331587(n) = k * reverse(k)
 *          (where reverse(k) is the binary reversal of k, A030101(k)).
 * https://oeis.org/A331589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A030101(A331588(n)). * A331588(n) is the k associated
 *            with the nth distinct product * of the form k * reverse(k)
 *            (where k is odd). * The sequence a(n) is the maximal k for each
 *            distinct product. * Implementation enumerates odd k, computes
 *            products, sorts them, * collapses duplicates keepin...
 * Range:     u8 none, u16 none, u32 n <= 1559, u64 n <= 8347, u128 n <= 8347,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Reverse the binary representation of an odd 64‑bit integer.
 * Leading zeros are dropped, which matches the OEIS definition
 * of the binary reversal A030101. */
static inline seq_t rev_bits(idx_t x)
{
    seq_t rev = 0;
    while (x) {
        rev = (rev << 1) | (x & 1);
        x >>= 1;
    }
    return rev;
}

/* Pair of product and its generating odd k. */
typedef struct {
    seq_t prod;
    idx_t k;
} Pair;

/* Comparison function for qsort: sort by product ascending,
 * then by k ascending (unused after the first sort). */
static int cmp_pair(const void *a, const void *b)
{
    const Pair *A = (const Pair *)a;
    const Pair *B = (const Pair *)b;
    if (A->prod < B->prod) return -1;
    if (A->prod > B->prod) return 1;
    return (A->k <= B->k) ? -1 : 1;
}

/* Generate the sequence a(·) for all odd k ≤ max_k_even.
 * The resulting array contains the maximal k for each distinct
 * product k*rev(k), sorted by that product.  The array is stored
 * in *out with length *len. */
static void generate_terms(idx_t max_k_even, seq_t **out, size_t *len)
{
    size_t odd_count = (max_k_even + 1) / 2;   /* number of odd integers ≤ max_k_even */
    Pair *pairs = (Pair *)malloc(sizeof(Pair) * odd_count);
    size_t idx = 0;
    for (idx_t k = 1; k <= max_k_even; k += 2) {
        seq_t rev = rev_bits(k);
        pairs[idx].prod = (seq_t)k * rev;
        pairs[idx].k   = k;
        idx++;
    }

    /* Sort by product so that equal products are adjacent. */
    qsort(pairs, odd_count, sizeof(Pair), cmp_pair);

    /* Count distinct products. */
    size_t distinct = 0;
    for (size_t i = 0; i < odd_count; i++) {
        if (i == 0 || pairs[i].prod != pairs[i - 1].prod)
            distinct++;
    }

    seq_t *arr = (seq_t *)malloc(sizeof(seq_t) * distinct);
    size_t pos = 0;
    size_t written = 0;
    while (pos < odd_count) {
        seq_t curProd = pairs[pos].prod;
        idx_t maxk = pairs[pos].k;
        pos++;
        while (pos < odd_count && pairs[pos].prod == curProd) {
            if (pairs[pos].k > maxk) maxk = pairs[pos].k;
            pos++;
        }
        arr[written++] = (seq_t)maxk;
    }

    *out = arr;
    *len = written;
    free(pairs);
}

/* Static cache holding the computed a(·) sequence. */
static seq_t *a_cache = NULL;
static size_t a_cache_len = 0;
static idx_t a_k_limit_even = 0;

/* Public function implementing a(·). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset 0; return 0 */

    /* Initial limit: enough for thousands of terms (≈100k odd k). */
    if (a_cache == NULL) {
        a_k_limit_even = 200000;        /* 200 000 ⇒ about 100 000 odd k */
        generate_terms(a_k_limit_even, &a_cache, &a_cache_len);
    }

    /* Grow the cache if the requested index exceeds the current size. */
    while ((size_t)n > a_cache_len) {
        a_k_limit_even *= 2;            /* double the range of odd k */
        free(a_cache);
        generate_terms(a_k_limit_even, &a_cache, &a_cache_len);
    }

    return a_cache[n - 1];
}
