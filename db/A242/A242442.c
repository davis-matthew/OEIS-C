/*
 * A242442  Number of ways of writing n, a positive integer, as an unordered
 *          sum of a triangular number (A000217), an odd square (A016754) and
 *          a pentagonal number (A000326).
 * https://oeis.org/A242442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Representation: a(n) counts unordered triples (T,S,P)
   with T a triangular number (including 0),
   S an odd square (1,9,25,…),
   P a pentagonal number (including 0)
   such that T + S + P = n (n ≥ 1). */

#include <stdlib.h>

/* static storage for precomputed data */
static idx_t max_n = 0;                 /* largest n that has been processed   */
static seq_t *pairCount = NULL;         /* pairCount[r] = # of (T,P) pairs with sum r   */
static seq_t *triangle_cache = NULL;    /* list of triangular numbers ≤ current max_n   */
static seq_t *pentagon_cache = NULL;    /* list of pentagonal numbers ≤ current max_n   */
static idx_t triangle_cnt = 0;
static idx_t pentagon_cnt = 0;

/* Extend precomputed tables up to new_max (new_max ≥ max_n) */
static void enlarge_tables(idx_t new_max) {
    /* Triangle numbers */
    idx_t tr_cnt = 0;
    for (idx_t k = 0;; ++k) {
        seq_t t = ((seq_t)k) * ((seq_t)k + 1) / 2;
        if (t > new_max) break;
        ++tr_cnt;
    }
    seq_t *new_tri = (seq_t*)malloc((size_t)tr_cnt * sizeof(seq_t));
    for (idx_t k = 0, j = 0;; ++k) {
        seq_t t = ((seq_t)k) * ((seq_t)k + 1) / 2;
        if (t > new_max) break;
        new_tri[j++] = t;
    }
    free(triangle_cache);
    triangle_cache = new_tri;
    triangle_cnt = tr_cnt;

    /* Pentagonal numbers (j starting at 0 gives 0) */
    idx_t pent_cnt = 0;
    for (idx_t j = 0;; ++j) {
        seq_t p = ((seq_t)j) * (3 * (seq_t)j - 1) / 2;
        if (p > new_max) break;
        ++pent_cnt;
    }
    seq_t *new_pent = (seq_t*)malloc((size_t)pent_cnt * sizeof(seq_t));
    for (idx_t j = 0, i = 0;; ++j) {
        seq_t p = ((seq_t)j) * (3 * (seq_t)j - 1) / 2;
        if (p > new_max) break;
        new_pent[i++] = p;
    }
    free(pentagon_cache);
    pentagon_cache = new_pent;
    pentagon_cnt = pent_cnt;

    /* pairCount array */
    size_t pc_size = (size_t)(new_max + 1);
    seq_t *new_pc = (seq_t*)calloc(pc_size, sizeof(seq_t));
    for (idx_t i = 0; i < tr_cnt; ++i) {
        seq_t tr_val = new_tri[i];
        for (idx_t j = 0; j < pent_cnt; ++j) {
            seq_t sum = tr_val + new_pent[j];
            if (sum <= new_max) {
                ++new_pc[(size_t)sum];
            }
        }
    }
    free(pairCount);
    pairCount = new_pc;
    max_n = new_max;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined for n < 1, return 0 */
    if (n > max_n) enlarge_tables(n);

    seq_t result = 0;
    for (idx_t m = 1;; ++m) {
        seq_t s = ((seq_t)(2 * m - 1)) * ((seq_t)(2 * m - 1));
        if (s > n) break;
        result += pairCount[(size_t)(n - s)];
    }
    return result;
}
