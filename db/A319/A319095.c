/*
 * A319095  Triangle read by rows: T(0,0) = 1; T(n,k) = T(n-1, k) + T(n-1,
 *          k-1) + T(n-1, k-2) + 3*T(n-1, k-3) + T(n-1, k-4) + T(n-1, k-5) +
 *          T(n-1, k-6) + T(n-1, k-7) for k = 0..7*n; T(n,k) = 0 for n or k <
 *          0.
 * https://oeis.org/A319095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS sequence A319095 – flattened triangle of (1 + x + x² + 3x³ + x⁴ + x⁵ + x⁶ + x⁷)^k.
 *
 *  a(n) = T(r,k) where r and k are chosen so that  
 *     n = Σ_{i=0}^{r-1}(7i+1) + k   (0 ≤ k ≤ 7r).  
 *  The triangle satisfies T(0,0)=1 and  
 *     T(r,k)=T(r-1,k)+T(r-1,k-1)+T(r-1,k-2)+3·T(r-1,k-3)+
 *            T(r-1,k-4)+T(r-1,k-5)+T(r-1,k-6)+T(r-1,k-7)
 *  with T(r,k)=0 for k<0 or k>7r.
 *
 *  The routine computes a(n) by dynamic programming up to the required row.
 */

#include <stddef.h>
#include <stdlib.h>

/* polynomial coefficients of the generating polynomial */
static const unsigned int COEFF[8] = {1, 1, 1, 3, 1, 1, 1, 1};

/*
 *  Return the n‑th element of the flattened triangle.
 */
seq_t a(idx_t n)
{
    /* Determine row r and column k corresponding to index n. */
    idx_t r = 0;
    idx_t cumulative = 0;

    for (;; ++r) {
        idx_t len = 7 * r + 1;
        if (cumulative + len > n)
            break;
        cumulative += len;
    }
    idx_t k = n - cumulative;
    (void)k;  /* will be used later as array index */

    /* Row 0 is a single entry 1. */
    if (r == 0)
        return (seq_t)1;

    /* Previous row array (initialised to 1 for row 0). */
    size_t prev_len = 1;
    seq_t *prev = (seq_t *)calloc(prev_len, sizeof(seq_t));
    if (!prev)
        return (seq_t)0;          /* allocation failure → 0 */
    prev[0] = (seq_t)1;

    /* Build rows up to r. */
    for (idx_t row = 1; row <= r; ++row) {
        size_t curr_len = 7 * row + 1;
        seq_t *curr = (seq_t *)calloc(curr_len, sizeof(seq_t));
        if (!curr) {
            free(prev);
            return (seq_t)0;
        }

        /* Add contributions from the previous row. */
        for (size_t i = 0; i < prev_len; ++i) {
            seq_t val = prev[i];
            if (val == 0)
                continue;
            for (int offset = 0; offset < 8; ++offset) {
                size_t idx = i + offset;
                if (idx < curr_len) {
                    /* multiplication is safe: seq_t is unsigned by definition. */
                    curr[idx] += val * (seq_t)COEFF[offset];
                }
            }
        }

        free(prev);
        prev = curr;
        prev_len = curr_len;
    }

    /* The desired value is at index k of the final row. */
    seq_t result = 0;
    if (k < prev_len)
        result = prev[k];

    free(prev);
    return result;
}
