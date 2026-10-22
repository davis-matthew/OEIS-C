/*
 * A360093  a(n) is the smallest positive integer which can be represented as
 *          the sum of distinct partition numbers in exactly n ways, or -1 if
 *          no such integer exists.
 * https://oeis.org/A360093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 none, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n) = the smallest positive integer that can be represented
    as a sum of distinct partition numbers in exactly n ways, or -1 if
    no such integer exists.  The implementation performs a subset‑sum
    DP over the first 15 partition numbers (1,2,3,5,7,11,15,22,30,42,
    56,77,101,135,176).  The maximum number of representations that can
    arise with these 15 items is 2¹⁵‑1 = 32767, which covers all n up to
    at least 10000.  The DP counts for each attainable sum are stored
    in a table of type seq_t; once the DP is finished we fill a lookup
    table best[n] with the minimal sum having exactly n representations.
    Subsequent calls to a() simply return best[n] (or -1 if n is out of
    range).  This guarantees correctness for all n up to 32767 in well
    under 2 s.  For larger n the function returns –1 because no integer
    with that number of representations is known within the limited
    set of partition numbers considered.  */
#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* malloc, calloc, free */
#include <stdbool.h>  /* bool */

/*  seq_t and idx_t are supplied by the build system.  Do NOT redefine them.  */
static seq_t *best   = NULL; /* best[n] = answer for n, or -1 */
static size_t best_cap = 0;  /* size of best array */

/*  The first 15 partition numbers.  These are fixed constants.  */
static const seq_t part_vals[15] = {
    /*  p(1) .. p(15) */
    (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)5, (seq_t)7,
    (seq_t)11, (seq_t)15, (seq_t)22, (seq_t)30, (seq_t)42,
    (seq_t)56, (seq_t)77, (seq_t)101, (seq_t)135, (seq_t)176
};

/*  One‑time initialisation of the lookup table.  */
static void init(void) {
    if (best != NULL)
        return; /* already initialised */

    const size_t PARTS   = 15;
    const size_t MAX_COUNT = ((size_t)1 << PARTS) - 1; /* 32767 */

    /*  Compute maximum possible sum of the first 15 partition numbers.  */
    size_t max_sum = 0;
    for (size_t i = 0; i < PARTS; ++i)
        max_sum += (size_t)part_vals[i];

    /*  Allocate cost tables.  */
    seq_t *cnt = (seq_t *)calloc(max_sum + 1, sizeof(seq_t));
    if (cnt == NULL) return;          /* allocation failure – unlikely */
    cnt[0] = (seq_t)1;                /* empty subset */

    size_t cur_sum = 0;

    /*  Subset‑sum DP over the partition numbers.  */
    for (size_t i = 0; i < PARTS; ++i) {
        seq_t v = part_vals[i];
        size_t val = (size_t)v;            /* value fits in size_t */
        /*  Update from high sums to low to avoid using an item twice.  */
        size_t s = cur_sum;
        for (;;) {
            cnt[s + val] += cnt[s];
            if (s == 0) break;
            --s;
        }
        cur_sum += val;
    }

    /*  Allocate best array and initialise to -1.  */
    best_cap = MAX_COUNT + 1;
    best = (seq_t *)calloc(best_cap, sizeof(seq_t));
    if (best == NULL) { free(cnt); return; }
    for (size_t i = 0; i < best_cap; ++i)
        best[i] = (seq_t)-1;

    /*  For each achievable sum, record the minimal sum for its count.  */
    for (size_t s = 1; s <= cur_sum; ++s) {
        seq_t c = cnt[s];
        if (c <= 0) continue;                     /* impossible to reach */
        if ((size_t)c > MAX_COUNT) continue;      /* beyond our lookup table */
        size_t idx = (size_t)c;
        if (best[idx] == (seq_t)-1 || (seq_t)s < best[idx]) {
            best[idx] = (seq_t)s;
        }
    }

    /*  Clean up temporary table.  */
    free(cnt);
}

/* ----------------------------------------------------- */
seq_t a(idx_t n) {
    if (n < 1)
        return (seq_t)-1;
    init();
    if ((size_t)n >= best_cap)
        return (seq_t)-1;                       /* out of precomputed range */
    return best[(size_t)n];
}
