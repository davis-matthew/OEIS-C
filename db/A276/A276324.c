/*
 * A276324  Antidiagonal indices of Greedy Queens (see A065188).
 * https://oeis.org/A276324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = antidiagonal index of the n-th queen in the greedy queens
 *            algorithm
 * Range:     u8 n <= 121, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


#define MAX_COORD 60000
#define MAX_RESULTS 50000
#define DIAG_OFFSET MAX_COORD

/* Track used lines */
static bool usedRow[MAX_COORD + 1] = {false};
static bool usedCol[MAX_COORD + 1] = {false};
static bool usedDiag[2 * MAX_COORD + 1] = {false};
static bool usedAntiDiag[2 * MAX_COORD + 3] = {false};

/* Cached results */
static idx_t computedN = 0;          /* largest n computed so far */
static idx_t nextK = 1;              /* next diagonal to try */
static seq_t results[MAX_RESULTS + 1] = {0};

/* Returns the antidiagonal index of the n-th queen (n ≥ 1).  */
seq_t a(idx_t n) {
    if (n == 0) return 0;                           /* undefined, safe fallback */

    /* Return already cached value if available */
    if (n <= computedN) return results[n];

    /* Simulate the greedy placement up to the required n */
    while (computedN < n) {
        for (idx_t k = nextK; ; ++k) {               /* k = antidiagonal index a(n) */
            if (usedAntiDiag[k + 1]) continue;       /* diagonal already occupied */

            bool placed = false;
            for (idx_t i = 1; i <= k; ++i) {         /* i ranges over rows on this diagonal */
                idx_t j = k + 1 - i;                 /* j = column corresponding to i */
                if (!usedRow[i] && !usedCol[j]) {
                    int diff = (int)i - (int)j;     /* main diagonal difference */
                    if (!usedDiag[diff + DIAG_OFFSET]) {
                        /* Place the queen */
                        usedRow[i] = true;
                        usedCol[j] = true;
                        usedDiag[diff + DIAG_OFFSET] = true;
                        usedAntiDiag[k + 1] = true;

                        ++computedN;
                        results[computedN] = (seq_t)k; /* store a(n) */
                        nextK = k + 1;                /* next search starts after this diagonal */
                        placed = true;
                        break;
                    }
                }
            }
            if (placed) break;                       /* move on to the next queen */
        }
    }

    return results[n];
}
