/*
 * A343598  Positive integers k such that exactly half the integers in [1..k]
 *          are divisible by a 7-smooth composite number.
 * https://oeis.org/A343598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = k, where k is the smallest integer such that exactly *
 *            half of the integers in [1..k] are divisible by a 7‑smooth
 *            composite. * The sequence is finite: a(136) = 1406. *
 *            Calculation: sieve all numbers up to 2000 that are divisible by
 *            * at least one 7‑smooth composite. For each even k, compar...
 * Range:     u8 n <= 47, u16 n <= 136, u32 n <= 136, u64 n <= 136,
 *            u128 n <= 136, big any n
 * Helpers:   is_7smooth_composite
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_7smooth_composite.h"

#define MAX_K 2000         /* upper bound for k (≥1406) */
#define MAX_COMPOSITES 500 /* generous limit for 7‑smooth composites */

/* static data structures */
static bool bad[MAX_K + 1] = {0};
static idx_t prefix[MAX_K + 1] = {0};
static idx_t terms[200] = {0};        /* holds the sequence values */
static size_t term_count = 0;
static bool init_done = false;

/* initialization: build the sequence once */
static void build_sequence(void)
{
    if (init_done) return;

    /* collect all 7‑smooth composites up to MAX_K */
    size_t comps = 0;
    uint64_t composite_list[MAX_COMPOSITES];
    for (uint64_t n = 4; n <= MAX_K; ++n) {
        if (is_7smooth_composite(n)) {
            if (comps < MAX_COMPOSITES) {
                composite_list[comps++] = n;
            }
        }
    }

    /* mark all multiples of each composite */
    for (size_t i = 0; i < comps; ++i) {
        uint64_t c = composite_list[i];
        for (uint64_t j = c; j <= MAX_K; j += c) {
            bad[(size_t)j] = true;
        }
    }

    /* build prefix sum of marked numbers */
    idx_t cnt = 0;
    for (size_t k = 1; k <= MAX_K; ++k) {
        if (bad[k]) ++cnt;
        prefix[k] = cnt;
    }

    /* collect terms: even k with prefix[k] == k/2 */
    for (size_t k = 2; k <= MAX_K; k += 2) {
        if (prefix[k] == (idx_t)(k / 2)) {
            terms[term_count++] = (idx_t)k;
            if (term_count >= 136) break;  /* known length of the sequence */
        }
    }

    init_done = true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* offset is 1; n==0 not defined */
    build_sequence();
    if ((size_t)n - 1 < term_count) {
        return (seq_t)terms[(size_t)n - 1];
    } else {
        return (seq_t)0;               /* undefined beyond known terms */
    }
}
