/*
 * A330461  Array read by antidiagonals where A(n,k) is the number of multiset
 *          partitions with k levels that are strict at all levels and have
 *          total sum n.
 * https://oeis.org/A330461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 175, u32 n <= 384, u64 n <= 913,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Compute OEIS A330461.
 * The array A(s,k) is defined by
 *    A(s,0) = 1  for all s ≥ 0
 *    A(s,k) = [x^s] ∏_{i≥1} (1 + x^i)^{A(i,k-1)}   for k ≥ 1
 * The sequence a(n) is the anti‑diagonal reading:
 *    let S = max{ m : m(m+1)/2 ≤ n }
 *    let d = n – S(S+1)/2
 *    a(n) = A(S-d, d)   (row = S-d, column = d)
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_ROW 200          /* covers S up to 200 (triangular(200)=20100) */

static bool table_initialized = false;
static seq_t A[MAX_ROW + 1][MAX_ROW + 1];

/* triangular number for small integers */
static inline uint32_t tri(uint32_t s)
{
    return (uint32_t)((seq_t)s * (s + 1) / 2);
}

/* initialise the table A(s,k) for 0 ≤ s,k ≤ MAX_ROW */
static void init_table(void)
{
    if (table_initialized) return;

    /* column 0: all ones */
    for (int s = 0; s <= MAX_ROW; ++s) A[s][0] = 1;

    seq_t poly[MAX_ROW + 1];
    seq_t next_poly[MAX_ROW + 1];
    seq_t comb[MAX_ROW + 1];

    for (int k = 1; k <= MAX_ROW; ++k) {
        /* start with polynomial 1 */
        for (int t = 0; t <= MAX_ROW; ++t) poly[t] = 0;
        poly[0] = 1;

        for (int i = 1; i <= MAX_ROW; ++i) {
            /* zero next polynomial */
            for (int t = 0; t <= MAX_ROW; ++t) next_poly[t] = 0;

            seq_t e = A[i][k - 1];          /* exponent for (1 + x^i) */
            int maxj = MAX_ROW / i;
            if (e < (seq_t)maxj) maxj = (int)e;

            /* binomial coefficients C(e,j) for j=0..maxj */
            comb[0] = 1;
            for (int j = 1; j <= maxj; ++j) {
                comb[j] = comb[j - 1] * (e - (seq_t)(j - 1)) / (seq_t)j;
            }

            /* convolution: poly * (1 + x^i)^e */
            for (int t = 0; t <= MAX_ROW; ++t) {
                seq_t curv = poly[t];
                if (!curv) continue;
                for (int j = 0; j <= maxj; ++j) {
                    int nt = t + i * j;
                    if (nt > MAX_ROW) break;
                    next_poly[nt] += curv * comb[j];
                }
            }

            /* prepare for next i */
            for (int s = 0; s <= MAX_ROW; ++s) poly[s] = next_poly[s];
        }

        /* store column k */
        for (int s = 0; s <= MAX_ROW; ++s) A[s][k] = poly[s];
    }

    table_initialized = true;
}

seq_t a(idx_t n)
{
    init_table();

    /* locate the anti‑diagonal containing n */
    uint32_t S = 0;
    while (tri(S + 1) <= n) ++S;
    uint32_t diff = (uint32_t)n - tri(S);

    uint32_t row = S - diff;
    uint32_t col = diff;

    if (row > (uint32_t)MAX_ROW || col > (uint32_t)MAX_ROW) return 0;
    return A[row][col];
}
