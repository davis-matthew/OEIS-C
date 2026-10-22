/*
 * A386950  A sequence constructed by greedily sampling the pdf
 *          (H(10)-H(i))/10, where H(i) is the i-th Harmonic number and the
 *          support is [0,9], to minimize discrepancy.
 * https://oeis.org/A386950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Greedy construction of A386950. At each step choose the k∈[0,9]
 *            that minimises (c(k)+1)/p(k) where c(k) are previous counts and
 *            p(k) are the probabilities (H(10)-H(k))/10
 * Range:     u8 n <= 9167, u16 n <= 9167, u32 n <= 9167, u64 n <= 9167,
 *            u128 n <= 9167, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The 10 probability numerators (denominator 25200) for k=0..9 */
static const uint64_t PNUM[10] = {
    7381, 4861, 3601, 2761, 2131,
    1627, 1207,  847,  532,  252
};

/* Static memoisation state */
static seq_t *VALUES = NULL;          /* array of already computed a(n) */
static idx_t   SIZE   = 0;            /* number of elements stored  (a(1) at VALUES[0]) */
static idx_t   CAP    = 0;            /* current allocation capacity */
static uint64_t CNT[10] = {0,0,0,0,0,0,0,0,0,0}; /* counts of each digit up to SIZE */

/* Increase the allocation to at least 'need' elements */
static void ensure_capacity(idx_t need)
{
    if (need <= CAP) return;
    idx_t new_cap = CAP ? CAP : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_vals = (seq_t*)realloc(VALUES, new_cap * sizeof(seq_t));
    if (!new_vals) exit(EXIT_FAILURE); /* allocation failed */
    VALUES = new_vals;
    CAP    = new_cap;
}

/* Greedy construction up to index 'n' */
static void build_up_to(idx_t n)
{
    for (idx_t i = SIZE; i < n; ++i) {
        int best = 0;
        /* Find k with minimal (CNT[k]+1)/PNUM[k] */
        for (int k = 1; k < 10; ++k) {
            uint64_t left  = (CNT[k] + 1ULL) * PNUM[best];
            uint64_t right = (CNT[best] + 1ULL) * PNUM[k];
            if (left < right) best = k;
        }
        VALUES[i] = (seq_t)best;
        CNT[best]++;
    }
    SIZE = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for n<1 */
    if (n > SIZE) {
        ensure_capacity(n);
        build_up_to(n);
    }
    return VALUES[n - 1];
}
