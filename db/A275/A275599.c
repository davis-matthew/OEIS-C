/*
 * A275599  Triangle read by rows: T(n,k) = number of right-skewed domino
 *          towers with n dominoes having a base of k dominoes placed
 *          end-to-end.
 * https://oeis.org/A275599
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = Sum_{i=1..k} (2*T(n-k,i) + A275204(n-k,i)), with
 *            T(n,k)=0 for n<=k; A275204: A(m,m)=1, A(m,k)=Sum_{i=1..k}
 *            (2*(k-i)+1)*A(m-k,i); a(n) reads T by rows starting at n=2 (row
 *            n has n-1 terms)
 * Range:     u8 n <= 30, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 67 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *R5_A = NULL;
static seq_t *R5_T = NULL;
static idx_t R5_rows = 0;   /* rows computed: 0..R5_rows */

static void R5_build(idx_t R)
{
    if (R5_A) { mem_free(R5_A); R5_A = NULL; }
    if (R5_T) { mem_free(R5_T); R5_T = NULL; }
    size_t w = (size_t)R + 1;
    R5_A = mem_new(seq_t, w * w);
    R5_T = mem_new(seq_t, w * w);
    R5_rows = R;
    for (size_t m = 1; m <= (size_t)R; ++m) {
        /* A row m */
        for (size_t k = 1; k <= m; ++k) {
            if (k == m) { R5_A[m * w + k] = (seq_t)1; continue; }
            seq_t s = 0;
            size_t r = m - k;
            for (size_t i = 1; i <= k && i <= r; ++i)
                s += (seq_t)(2 * (k - i) + 1) * R5_A[r * w + i];
            R5_A[m * w + k] = s;
        }
        /* T row m, k < m */
        for (size_t k = 1; k < m; ++k) {
            seq_t s = 0;
            size_t r = m - k;
            for (size_t i = 1; i <= k && i <= r; ++i)
                s += (seq_t)2 * R5_T[r * w + i] + R5_A[r * w + i];
            R5_T[m * w + k] = s;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    /* row r starts at index 2 + (r-2)(r-1)/2 and has r-1 terms */
    idx_t r = 2;
    for (;;) {
        idx_t start = 2 + (r - 2) * (r - 1) / 2;
        idx_t next = start + (r - 1);
        if (n < next) {
            idx_t k = n - start + 1;
            if (R5_rows < r) {
                idx_t want = r;
                if (want < 2 * R5_rows) want = 2 * R5_rows;
                if (want < 16) want = 16;
                R5_build(want);
            }
            return R5_T[(size_t)r * ((size_t)R5_rows + 1) + (size_t)k];
        }
        ++r;
    }
}
