/*
 * A330460  Triangle read by rows where T(n,k) is the number of set partitions
 *          with k blocks and total sum n.
 * https://oeis.org/A330460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A330460 – triangle T(n,k) of set partitions with k blocks and
 *            total sum n. The sequence a(n) is obtained by reading the
 *            triangle row‑by‑row, i.e. a(n) = T( row(n), col(n) ) where
 *            row(n) = ⌊(√(1+8n) – 1)/2⌋ col(n) = n – row(n)*(row(n)+1)/2
 * Range:     u8 n <= 191, u16 n <= 863, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   index_to_pos
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include necessary standard headers */
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include <stdlib.h>
#include "../utils/index_to_pos.h"

/* User supplied types seq_t and idx_t – do NOT define them here */

/* Internal constants */
static const idx_t MAX_SUM = 10000;            /* maximum sum n needed (row value) */
static size_t MAX_I;                          /* maximal number of distinct parts */

static seq_t *P;                              /* P[m][i] : partitions of m into i parts (unrestricted) */
static seq_t *S;                              /* Stirling numbers of the second kind S[i][k] */
static bool initialized = false;

/* Helper to allocate 2‑D array flattened into 1‑D */
static void init(void)
{
    if (initialized) return;

    /* Compute maximal number of distinct parts required for sums up to MAX_SUM. */
    MAX_I = (size_t)((sqrt(8.0 * (double)MAX_SUM + 1.0) - 1.0) / 2.0);

    /* Allocate partition table P[m][i] : (MAX_SUM+1) × (MAX_I+1) */
    P = (seq_t*)malloc((MAX_SUM + 1) * (MAX_I + 1) * sizeof(seq_t));
    if (!P) abort();

    /* Allocate Stirling table S[i][k] : (MAX_I+1) × (MAX_I+1) */
    S = (seq_t*)malloc((MAX_I + 1) * (MAX_I + 1) * sizeof(seq_t));
    if (!S) abort();

    /* Initialize all entries to zero */
    for (size_t idx = 0; idx < (MAX_SUM + 1) * (MAX_I + 1); ++idx) P[idx] = 0;
    for (size_t idx = 0; idx < (MAX_I + 1) * (MAX_I + 1); ++idx) S[idx] = 0;

    /* Base case for partitions: p(0,0)=1 */
    P[0 * (MAX_I + 1) + 0] = 1;

    /* Fill partition table: p(m,i) = p(m-1,i-1) + p(m-i,i) */
    for (size_t m = 1; m <= MAX_SUM; ++m) {
        for (size_t i = 1; i <= MAX_I; ++i) {
            seq_t val = 0;
            /* term from p(m-1, i-1) */
            val += P[(m - 1) * (MAX_I + 1) + (i - 1)];
            /* term from p(m-i, i) if m >= i */
            if (m >= i) val += P[(m - i) * (MAX_I + 1) + i];
            P[m * (MAX_I + 1) + i] = val;
        }
    }

    /* Stirling numbers of the second kind: S(0,0)=1, S(i,0)=0 for i>0 */
    S[0 * (MAX_I + 1) + 0] = 1;
    for (size_t i = 1; i <= MAX_I; ++i) {
        for (size_t k = 1; k <= i; ++k) {
            /* S(i,k) = k*S(i-1,k) + S(i-1,k-1) */
            seq_t val = k * S[(i - 1) * (MAX_I + 1) + k] + S[(i - 1) * (MAX_I + 1) + (k - 1)];
            S[i * (MAX_I + 1) + k] = val;
        }
    }

    initialized = true;
}

/* Return the sequence value a(n) */
seq_t a(idx_t n)
{
    init();                            /* Ensure tables are ready */

    idx_t row, col;
    index_to_pos(n, &row, &col);

    if (col > row) return (seq_t)0;    /* out of bounds in the triangle */

    /* Compute T(row, col) = sum_{i=col..row} A060016(row,i)*S(i, col)   */
    seq_t sum = 0;

    /* i cannot exceed MAX_I; also i <= row, but for i>row the term is zero */
    idx_t max_i = row < MAX_I ? row : MAX_I;
    for (idx_t i = col; i <= max_i; ++i) {
        /* Compute A060016: number of partitions of row into i distinct parts */
        /* This equals number of unrestricted partitions of: m = row - i*(i-1)/2 into i parts */
        idx_t offset = i * (i - 1) / 2;
        if (row < offset) continue;    /* impossible for this i */

        idx_t m = row - offset;
        seq_t part = P[m * (MAX_I + 1) + i];

        seq_t stir = S[i * (MAX_I + 1) + col];
        sum += part * stir;
    }

    return sum;
}
