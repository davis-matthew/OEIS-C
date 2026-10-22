/*
 * A319658  a(n) is the minimal number of successive ON cells that appears in
 *          n-th generation of rule-30 1D cellular automaton started from a
 *          single ON cell.
 * https://oeis.org/A319658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal run of consecutive ON cells in the * n‑th
 *            generation of Rule‑30 CA started from a single ON cell
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    /* ----- static state: previous row, current row, min run values ----- */
    static idx_t max_computed = 0;          /* largest n already processed   */
    static unsigned char *rowPrev = NULL;  /* row of generation max_computed  */
    static unsigned char *rowCurr = NULL;  /* temporary buffer for next gen   */
    static size_t rowCap = 0;              /* capacity of row buffers        */
    static seq_t *minRun = NULL;           /* minRun[0] unused, minRun[1] = 1 */
    static size_t minRunSize = 0;          /* allocated slots for minRun      */

    /* ----- first‑time initialization ----- */
    if (max_computed == 0) {
        rowCap = 1;
        rowPrev = (unsigned char*)malloc(rowCap);
        rowCurr = (unsigned char*)malloc(rowCap);
        if (!rowPrev || !rowCurr) return (seq_t)0;

        rowPrev[0] = 1;         /* single ON cell in generation 1 */
        minRunSize = 2;         /* indices 0 and 1 */
        minRun = (seq_t*)calloc(minRunSize, sizeof(seq_t));
        if (!minRun) return (seq_t)0;
        minRun[1] = (seq_t)1;
        max_computed = 1;
    }

    /* ----- return a(n) if already known ----- */
    if (n <= max_computed) return minRun[(size_t)n];

    /* ----- grow the minRun array if necessary ----- */
    if (minRunSize <= (size_t)n) {
        size_t newSize = n + 2;  /* keep index 0 unused */
        seq_t *tmp = (seq_t*)realloc(minRun, newSize * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        minRun = tmp;
        minRunSize = newSize;
    }

    /* ----- iterate generations from max_computed+1 to n ----- */
    for (idx_t gen = max_computed + 1; gen <= n; ++gen) {
        /* Length of the previous and current rows */
        size_t prevLen = 2u * (size_t)(gen - 1) - 1u;
        size_t currLen = prevLen + 2u;

        /* Resize buffers if necessary */
        if (rowCap < currLen) {
            rowCap = currLen;
            unsigned char *tmpA = (unsigned char*)realloc(rowPrev, rowCap);
            unsigned char *tmpB = (unsigned char*)realloc(rowCurr, rowCap);
            if (!tmpA || !tmpB) return (seq_t)0;
            rowPrev = tmpA;
            rowCurr = tmpB;
        }

        /* Apply Rule‑30: cell = left XOR (center OR right) */
        for (size_t k = 0; k < currLen; ++k) {
            unsigned char left  = 0, center = 0, right = 0;

            if (k >= 1u && (k - 1u) < prevLen) center = rowPrev[k - 1u];
            if (k >= 2u && (k - 2u) < prevLen) left   = rowPrev[k - 2u];
            if (k < prevLen)                      right = rowPrev[k];

            rowCurr[k] = left ^ (center | right);
        }

        /* Find minimal consecutive run of ON cells in this row */
        seq_t rowMin = (seq_t)currLen;  /* upper bound */
        unsigned int runLen = 0;
        for (size_t k = 0; k < currLen; ++k) {
            if (rowCurr[k]) {
                ++runLen;
            } else {
                if (runLen) {
                    if ((seq_t)runLen < rowMin) rowMin = (seq_t)runLen;
                    runLen = 0;
                }
            }
        }
        if (runLen && ((seq_t)runLen < rowMin)) rowMin = (seq_t)runLen;

        minRun[(size_t)gen] = rowMin;

        /* Prepare for next iteration: swap buffers */
        unsigned char *tmp = rowPrev;
        rowPrev = rowCurr;
        rowCurr = tmp;
    }

    max_computed = n;
    return minRun[(size_t)n];
}
