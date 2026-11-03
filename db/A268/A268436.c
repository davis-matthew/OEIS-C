/*
 * A268436  Triangle read by rows, T(n,k) = RF(n-k+1, n-k) * S1(n,k) where RF
 *          denotes the rising factorial and S1 the Stirling cycle numbers,
 *          for n>=0 and 0<=k<=n.
 * https://oeis.org/A268436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 36, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 44 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A268436
   Triangle read by rows: T(n,k) = RF(n-k+1,n-k) * S1(n,k)
   Sequence a(n) is the triangle flattened row‑wise.
   Recurrence used to compute T(n,k):
   T(n,k) = 1            if k == n
             0            if k == 0 && n > 0
             (n-1)*(4*(n-k)-2)*T(n-1,k) + T(n-1,k-1)  otherwise
*/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2  */
    idx_t r = 0;
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = (low + high) / 2;
        idx_t val = mid * (mid + 1) / 2;
        if (val <= n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    idx_t k = n - r * (r + 1) / 2;

    /* r = 0 => only entry is 1 */
    if (r == 0) return (seq_t)1;

    /* Allocate two rows of size r+1 */
    seq_t *prev = mem_new(seq_t, (r + 1));
    seq_t *next = mem_new(seq_t, (r + 1));
    if (!prev || !next) return (seq_t)0; /* malloc failed, return 0 */

    prev[0] = (seq_t)1;

    for (idx_t i = 1; i <= r; ++i) {
        next[0] = (seq_t)0;               /* T(i,0) = 0 for i>=1 */
        for (idx_t j = 1; j < i; ++j) {  /* 1 <= j <= i-1 */
            seq_t coeff = seq_mul((seq_t)(i - 1),
                                  (seq_t)(4 * (i - j) - 2));
            seq_t term  = seq_mul(coeff, prev[j]);  /* (i-1)*(4*(i-j)-2)*T(i-1,j) */
            next[j] = term + prev[j - 1];           /* + T(i-1,j-1) */
        }
        next[i] = (seq_t)1;                 /* T(i,i) = 1 */

        /* swap prev and next for next iteration */
        seq_t *tmp = prev;
        prev = next;
        next = tmp;
    }

    seq_t ans = prev[k];
    mem_free(prev);
    mem_free(next);
    return ans;
}
