/*
 * A321498  Numbers which can be written in at least two ways in the form
 *          (x-y)*(x^2-y^2) with x > y > 0.
 * https://oeis.org/A321498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 3971, u32 n <= 9313, u64 n <= 9313,
 *            u128 n <= 9313, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Enumerate all values v = d²·(d+2y) with d>0, y>0, collect
   the values that occur for at least two distinct pairs (d,y),
   sort them, and return the n‑th element of this sorted list. */
#include <stdlib.h>
#include <stdbool.h>

/* qsort comparison for seq_t values. */
static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *va = (const seq_t *)a;
    const seq_t *vb = (const seq_t *)b;
    if (*va < *vb) return -1;
    if (*va > *vb) return 1;
    return 0;
}

/* Cached sequence data. */
static seq_t *seqVals          = NULL;  /* sorted values with ≥2 reps   */
static idx_t seqSize           = 0;    /* number of such values         */
static idx_t seqCapacity       = 0;    /* allocated capacity             */
static idx_t currentBound      = 0;    /* last bound used in generation  */
static bool   initialized      = false;

/* Generate all values v = d²·(d+2y) with v ≤ bound, and fill seqVals
   with those that have at least two representations. */
static void generate_up_to(idx_t bound)
{
    /* Determine maximum d such that d²·(d+2) ≤ bound. */
    idx_t dmax = 0;
    while (1) {
        seq_t d_sq  = (seq_t)(dmax + 1) * (seq_t)(dmax + 1);
        seq_t lim   = d_sq * ((seq_t)(dmax + 1) + 2);
        if (lim > (seq_t)bound) break;
        dmax++;
    }

    /* Count total representations up to the bound. */
    idx_t pairCount = 0;
    for (idx_t d = 1; d <= dmax; ++d) {
        seq_t d_sq  = (seq_t)d * (seq_t)d;
        seq_t rest  = (seq_t)bound / d_sq;
        if (rest <= (seq_t)d) continue;
        seq_t ymax  = (rest - (seq_t)d) / 2;
        pairCount += (idx_t)ymax;
    }

    /* Allocate space for all values. */
    seq_t *allVals = (seq_t *)malloc(pairCount * sizeof(seq_t));
    seq_t *p = allVals;

    /* Generate all representations. */
    for (idx_t d = 1; d <= dmax; ++d) {
        seq_t d_sq  = (seq_t)d * (seq_t)d;
        seq_t rest  = (seq_t)bound / d_sq;
        if (rest <= (seq_t)d) continue;
        seq_t ymax  = (rest - (seq_t)d) / 2;
        for (seq_t y = 1; y <= ymax; ++y) {
            seq_t v = d_sq * ((seq_t)2 * y + (seq_t)d);
            *p++ = v;
        }
    }

    /* Sort the generated values. */
    qsort(allVals, pairCount, sizeof(seq_t), cmp_seq_t);

    /* Free any previously stored sequence data. */
    if (seqVals) free(seqVals);
    seqVals = NULL;
    seqSize = 0;
    seqCapacity = 0;

    /* Scan sorted array for values with frequency ≥2. */
    for (idx_t i = 0; i < pairCount; ) {
        idx_t j = i + 1;
        while (j < pairCount && allVals[j] == allVals[i]) ++j;
        if (j - i >= 2) {
            if (seqSize >= seqCapacity) {
                seqCapacity = seqCapacity ? seqCapacity * 2 : 16;
                seqVals = (seq_t *)realloc(seqVals, seqCapacity * sizeof(seq_t));
            }
            seqVals[seqSize++] = allVals[i];
        }
        i = j;
    }

    free(allVals);
    currentBound = bound;
    initialized  = true;
}

/* Ensure that the sequence contains at least n terms. */
static void ensure_terms(idx_t n)
{
    if (seqSize >= n) return;
    idx_t bound = currentBound ? currentBound * 2 : 2000000;
    while (1) {
        generate_up_to(bound);
        if (seqSize >= n) break;
        bound *= 2;
    }
}

/* Return the nth term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_terms(n);
    return seqVals[n - 1];
}
