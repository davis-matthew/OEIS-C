/*
 * A285736  a(n) = A285735(n) - A285734(n) = n - 2*A285734(n).
 * https://oeis.org/A285736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - 2*A285734(n), where A285734(n) is the largest
 *            squarefree x <= n/2 with n-x squarefree (a(1)=1); computed by a
 *            squarefree sieve and downward search
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 135 terms through n = 9729 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    bool *sf = (bool *)malloc((size_t)(n + 1) * sizeof(bool));
    if (!sf) abort();
    for (idx_t i = 0; i <= n; ++i) sf[i] = true;
    for (idx_t d = 2; d * d <= n; ++d) {
        idx_t sq = d * d;
        for (idx_t j = sq; j <= n; j += sq)
            sf[j] = false;
    }

    idx_t best = 0;
    for (idx_t x = n / 2; x >= 1; --x) {
        if (sf[x] && sf[n - x]) {
            best = x;
            break;
        }
    }
    free(sf);
    return (seq_t)n - (seq_t)2 * (seq_t)best;
}
