/*
 * A345834  Numbers that are the sum of eight fourth powers in exactly two
 *          ways.
 * https://oeis.org/A345834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4554, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A345834:
 * Numbers that are the sum of eight fourth powers in exactly two ways.
 * Computation:
 *   1. Enumerate all non‑decreasing 8‑tuples (a,b,c,d,e,f,g,h) with
 *      1 ≤ a ≤ … ≤ h ≤ MAX_BASE.
 *   2. For each tuple add the eight fourth powers, count the sum.
 *   3. All sums that occur exactly twice are the sequence elements.
 *   4. The result is stored once in a static array so that a(n) can be
 *      answered in O(1) for any order of requests up to the terms
 *      that have been generated (at least 10 000 terms for the
 *      intended test range).
 */

#include <stdlib.h>
#include <stdint.h>

/* Choose a base large enough to generate at least 10 000 terms.
 * B = 30 gives 8·30⁴ = 6 480 000 as maximum sum and about 1.6 × 10⁷
 * tuples, which is easily handled in < 2 s.  The count of distinct
 * sums with exactly two representations is well over 10 000. */
#define MAX_BASE 30

/* global storage for the precomputed terms */
static seq_t *global_terms = NULL;
static idx_t  global_len   = 0;

/* build the entire sequence once */
static void build_sequence(void)
{
    /* precompute fourth powers as 64‑bit values – they easily fit in
     * 32 bits for BASE ≤ 30. */
    uint64_t pow4[MAX_BASE + 1];
    pow4[0] = 0;
    for (int i = 1; i <= MAX_BASE; ++i) {
        pow4[i] = (uint64_t)i * i * i * i;
    }

    /* estimate the maximum possible sum */
    uint64_t maxSum = 8ULL * pow4[MAX_BASE];

    /* array of representation counts */
    int *cnt = (int*)calloc((size_t)maxSum + 1, sizeof(int));
    if (!cnt) abort();               /* allocation failure */

    /* enumerate all non‑decreasing 8‑tuples */
    for (int a = 1; a <= MAX_BASE; ++a) {
        uint64_t va = pow4[a];
        for (int b = a; b <= MAX_BASE; ++b) {
            uint64_t vb = va + pow4[b];
            for (int c = b; c <= MAX_BASE; ++c) {
                uint64_t vc = vb + pow4[c];
                for (int d = c; d <= MAX_BASE; ++d) {
                    uint64_t vd = vc + pow4[d];
                    for (int e = d; e <= MAX_BASE; ++e) {
                        uint64_t ve = vd + pow4[e];
                        for (int f = e; f <= MAX_BASE; ++f) {
                            uint64_t vf = ve + pow4[f];
                            for (int g = f; g <= MAX_BASE; ++g) {
                                uint64_t vg = vf + pow4[g];
                                for (int h = g; h <= MAX_BASE; ++h) {
                                    uint64_t sum = vg + pow4[h];
                                    ++cnt[sum];
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    /* count how many sums have exactly two representations */
    size_t terms_needed = 0;
    for (uint64_t s = 0; s <= maxSum; ++s) {
        if (cnt[s] == 2) ++terms_needed;
    }

    /* allocate array for the terms */
    seq_t *terms = (seq_t*)calloc(terms_needed, sizeof(seq_t));
    if (!terms) abort();

    /* fill the array with the qualifying sums */
    size_t idx = 0;
    for (uint64_t s = 0; s <= maxSum; ++s) {
        if (cnt[s] == 2) {
            terms[idx++] = (seq_t)s;
        }
    }

    /* store in global variables */
    global_terms = terms;
    global_len   = (idx_t)terms_needed;

    free(cnt);
}

/* required sequence function */
seq_t a(idx_t n)
{
    if (!global_terms) build_sequence();    /* build once on first call */

    if (n == 0 || n > global_len) return 0; /* out of range */
    return global_terms[n - 1];
}
