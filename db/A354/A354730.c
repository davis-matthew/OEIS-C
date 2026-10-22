/*
 * A354730  E.g.f. A(x) satisfies: A(x) = 1 + x * A(-log(1-x)).
 * https://oeis.org/A354730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * sum_{k=0}^{n-1} |Stirling1(n-1,k)| * a(k), a(0) = 1.
 *            The unsigned Stirling numbers of the first kind satisfy s(n,k) =
 *            s(n-1,k-1) + (n-1)*s(n-1,k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoised values of the sequence. */
    static seq_t *a_cache = NULL;  /* a_cache[i] = a(i) */
    static size_t a_cap  = 0;      /* capacity of a_cache */
    static idx_t  a_len  = 0;      /* maximum index stored (inclusive) */

    /* Current row of unsigned Stirling numbers of the first kind:
       s_row[k] = |Stirling1(curr_n, k)|, where curr_n = a_len. */
    static seq_t *s_row   = NULL;
    static size_t s_cap   = 0;
    static idx_t  s_row_n = 0;      /* equals a_len for the current row */

    /* Initialise on first call. */
    if (a_cache == NULL) {
        a_cap = 1;
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_cache[0] = (seq_t)1;       /* a(0) = 1 */
        a_len  = 0;

        s_cap   = 1;
        s_row   = (seq_t *)malloc(s_cap * sizeof(seq_t));
        s_row[0] = (seq_t)1;         /* s(0,0) = 1 */
        s_row_n = 0;
    }

    /* If the requested value is cached, return it. */
    if (n <= a_len) {
        return a_cache[n];
    }

    /* Grow caches to at least n. */
    for (idx_t m = a_len + 1; m <= n; ++m) {
        /* Compute a(m) using the previous Stirling row s_row. */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= s_row_n; ++k) {
            sum += s_row[k] * a_cache[k];
        }
        seq_t val = (seq_t)m * sum;

        /* Resize a_cache if necessary. */
        if ((size_t)m >= a_cap) {
            size_t new_cap = a_cap ? a_cap * 2 : 1;
            while (new_cap <= (size_t)m) new_cap <<= 1;
            a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
            a_cap   = new_cap;
        }
        a_cache[m] = val;
        a_len = m;

        /* Resize s_row if necessary. */
        if ((size_t)m + 1 > s_cap) {
            size_t new_cap = s_cap ? s_cap * 2 : 1;
            while (new_cap <= (size_t)m + 1) new_cap <<= 1;
            s_row = (seq_t *)realloc(s_row, new_cap * sizeof(seq_t));
            s_cap = new_cap;
        }
        /* Ensure the new element exists. */
        s_row[m] = (seq_t)0;

        /* Update Stirling row from high to low to avoid overwriting. */
        for (idx_t k = m; k >= 1; --k) {
            s_row[k] = s_row[k-1] + (seq_t)(m-1) * s_row[k];
        }
        s_row[0] = (seq_t)0;    /* s(m,0) = 0 for m > 0 */
        s_row_n = m;
    }

    return a_cache[n];
}
