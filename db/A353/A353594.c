/*
 * A353594  Triangle read by rows, T(n, k) = Sum_{j=0..n} binomial(j,
 *          k)*A352687(n, j).
 * https://oeis.org/A353594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = Sum_{j=0..n} binomial(j,k)*A352687(n,j), where
 *            A352687(n,j) = binomial(n,j)*binomial(n-1,j-1)/(n-j+1) -
 *            2*binomial(n-1,j)*binomial(n-1,j-2)/(n-1) (Narayana-related,
 *            A352687(0,0)=1, A352687(n,0)=0 for n>=1); row n of the result is
 *            read at flat index n(n+1)/2+k
 * Range:     u8 n <= 21, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-10)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"

static seq_t A353594_Tnk(idx_t n, idx_t k) {
    seq_t term1 = binomial((idx_t)n, (idx_t)k);
    term1 *= binomial((idx_t)n - 1, (idx_t)k - 1);
    term1 /= (seq_t)(n - k + 1);

    seq_t term2 = 0;
    if (k >= 2 && k <= n - 1) {
        term2 = 2 * binomial((idx_t)n - 1, (idx_t)k);
        term2 *= binomial((idx_t)n - 1, (idx_t)k - 2);
        term2 /= (seq_t)(n - 1);
    }
    return term1 - term2;
}

seq_t a(idx_t n) {
    idx_t t = 0;
    while ((t + 1) * (t + 2) / 2 <= n) t++;
    idx_t k = n - t * (t + 1) / 2;

    seq_t sum = 0;
    if (t == 0) {
        return 1;
    }
    for (idx_t j = 1; j <= t; j++) {
        if (j < k) continue;
        sum += binomial(j, k) * A353594_Tnk(t, j);
    }
    return sum;
}
