/*
 * A396791  a(n) = Sum_{k=1..2*n-1} 2*k!*Stirling2(2*n-1,k)/(k+1).
 * https://oeis.org/A396791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..2*n-1} 2*k!*Stirling2(2*n-1,k)/(k+1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    const idx_t m = 2 * n - 1;          /* highest row index */

#if defined(__cplusplus)
    /* C++ implementation: use std::vector for dynamic storage */
    std::vector<seq_t> fact(m + 1);
    std::vector<seq_t> prev(m + 1, 0), cur(m + 1, 0);
    fact[0] = 1;
    for (idx_t i = 1; i <= m; ++i)
        fact[i] = fact[i - 1] * i;     /* factorials */

    prev[0] = 1;                       /* S(0,0) = 1 */

    seq_t result = 0;
    for (idx_t row = 1; row <= m; ++row) {
        cur[0] = 0;
        for (idx_t k = 1; k <= row; ++k)
            cur[k] = prev[k] * k + prev[k - 1];

        if ((row & 1U) && ((row + 1) / 2) == n) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= row; ++k) {
                /* Use 128‑bit arithmetic for the intermediate product */
                unsigned __int128 term =
                    (unsigned __int128)fact[k] *
                    (unsigned __int128)cur[k] * 2U;
                term /= (k + 1);
                sum += (seq_t)term;
            }
            result = sum;
        }

        std::swap(prev, cur);
    }
    return result;
#else
    /* Plain C implementation */
    seq_t *fact = (seq_t *)malloc((m + 1) * sizeof(seq_t));
    if (!fact) abort();
    seq_t *prev = (seq_t *)calloc(m + 1, sizeof(seq_t));
    seq_t *cur  = (seq_t *)calloc(m + 1, sizeof(seq_t));
    if (!prev || !cur) abort();

    fact[0] = 1;
    for (idx_t i = 1; i <= m; ++i)
        fact[i] = fact[i - 1] * i;

    prev[0] = 1;                      /* S(0,0) = 1 */

    seq_t result = 0;
    for (idx_t row = 1; row <= m; ++row) {
        cur[0] = 0;
        for (idx_t k = 1; k <= row; ++k)
            cur[k] = prev[k] * k + prev[k - 1];

        if ((row & 1U) && ((row + 1) / 2) == n) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= row; ++k) {
                unsigned __int128 term =
                    (unsigned __int128)fact[k] *
                    (unsigned __int128)cur[k] * 2U;
                term /= (k + 1);
                sum += (seq_t)term;
            }
            result = sum;
        }

        /* swap prev and cur */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    free(fact);
    free(prev);
    free(cur);
    return result;
#endif
}
