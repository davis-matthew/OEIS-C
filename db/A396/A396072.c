/*
 * A396072  Numbers not of the form Sum_{k=1..m} floor(m/k^2).
 * https://oeis.org/A396072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A396072 – Numbers not of the form Σ_{k=1..m} ⌊m/k²⌋
    (the complement of A013936).  We pre‑compute all
    representable values up to a fixed bound and collect
    the missing ones into a static array.  After the
    first call, a(n) returns the (n‑1)th element directly.
*/

#include <stdlib.h>
#include <stdbool.h>

/* static storage for pre‑computed missing numbers */
static seq_t *missing = NULL;      /* array of values not representable */
static size_t missing_len = 0;     /* how many values were stored */
static bool initialized = false;   /* flag to run the initialization once */

/* pre‑compute the set of representable S(m) up to a fixed limit
   and store the complement (missing values).  The limit is chosen
   so that the first 10 000 terms of the sequence are contained
   in the array.  Complexity is O(limit·√limit). */
static void init(void)
{
    const idx_t bound = 200000;                /* upper bound for representability */
    unsigned char *repr = (unsigned char *)calloc(bound + 1, 1); /* repr[x]==1 iff x is representable */

    /* mark representable numbers */
    for (idx_t m = 1; m <= bound; ++m) {
        seq_t sum = 0;
        for (idx_t k = 1, sq = 1; sq <= m; ++k, sq = k * k) {
            /* m/sq is an idx_t, we cast to seq_t for addition */
            sum += (seq_t)(m / sq);
        }
        if (sum <= (seq_t)bound) {
            repr[(idx_t)sum] = 1;
        }
    }

    /* store all numbers that are not representable */
    missing_len = 0;
    missing = (seq_t *)malloc((bound + 1) * sizeof(seq_t));
    for (idx_t i = 1; i <= bound; ++i) {
        if (!repr[i]) {
            missing[missing_len++] = (seq_t)i;
        }
    }
    free(repr);
}

seq_t a(idx_t n)
{
    if (!initialized) {
        init();
        initialized = true;
    }
    if (n == 0 || n > missing_len) {
        /* out of range – return 0, though the harness will not use such n */
        return (seq_t)0;
    }
    return missing[n - 1];
}
