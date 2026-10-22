/*
 * A371234  E.g.f. satisfies A(x) = 1 - x^2*A(x)^4*log(1 - x*A(x)^2).
 * https://oeis.org/A371234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! (2n)! * Σ_{k=0..⌊n/3⌋} |s(n-2k,k)| / ((n-2k)!
 *            (2n-k+1)!)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses unsigned Stirling numbers of the first kind (absolute values). */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"

/* ----------  Stirling numbers of the first kind ------------- */
static seq_t **_stirling_rows      = NULL;   /* rows of Stirling numbers */
static idx_t   _stirling_max_n     = 0;      /* highest n for which rows are computed */

/* Ensure that Stirling numbers are available up to idx N (inclusive). */
static void _ensure_stirling_upto(idx_t N)
{
    if (_stirling_rows != NULL && _stirling_max_n >= N)
        return;  /* already computed */

    /* Initialize row 0 if not present */
    if (_stirling_rows == NULL) {
        _stirling_rows = (seq_t**)malloc(sizeof(seq_t*));
        _stirling_rows[0] = alloc_seq_array(1);
        _stirling_rows[0][0] = (seq_t)1;   /* s(0,0)=1 */
        _stirling_max_n = 0;
    }

    /* Build rows up to N */
    for (idx_t i = _stirling_max_n + 1; i <= N; ++i) {
        size_t cur_len = (size_t)i + 1;
        seq_t *cur = alloc_seq_array(cur_len);

        seq_t *prev = _stirling_rows[i - 1];
        size_t prev_len = (size_t)i;  /* previous row length (i) */

        for (size_t k = 0; k < cur_len; ++k) {
            seq_t term1 = (k > 0 && k - 1 < prev_len) ? prev[k - 1] : (seq_t)0;
            seq_t term2 = (k < prev_len) ? prev[k] * (seq_t)(i - 1) : (seq_t)0;
            cur[k] = term1 + term2;
        }

        /* Expand the row pointer array */
        _stirling_rows = (seq_t**)realloc(_stirling_rows,
                                          (size_t)(i + 1) * sizeof(seq_t*));
        _stirling_rows[i] = cur;
        _stirling_max_n = i;
    }
}

/* Retrieve |s(n,k)|, zero if k>n. */
static seq_t _stirling_abs(idx_t n, idx_t k)
{
    if (k > n)
        return (seq_t)0;
    return _stirling_rows[n][k];
}

/* ----------  Main sequence function  ----------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    /* Prepare required Stirling numbers up to n */
    _ensure_stirling_upto(n);

    seq_t sum = (seq_t)0;
    idx_t maxk = n / 3;

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t m = n - 2 * k;              /* m = n - 2k >= 0 */
        seq_t stir = _stirling_abs(m, k); /* |s(m,k)| */
        if (stir == (seq_t)0)
            continue;

        /* Product P1 = Π_{i=m+1}^{n} i  (empty product is 1 when m==n) */
        seq_t term = stir;
        for (idx_t i = m + 1; i <= n; ++i)
            term *= (seq_t)i;

        /* Product P2 = Π_{i=2n-k+2}^{2n} i  (empty when start > 2n) */
        idx_t start = 2 * n - k + 2;
        for (idx_t i = start; i <= 2 * n; ++i)
            term *= (seq_t)i;

        sum += term;
    }

    return sum;
}
