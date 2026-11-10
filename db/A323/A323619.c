/*
 * A323619  Expansion of e.g.f. 1 - LambertW(-log(1+x))*(2 +
 *          LambertW(-log(1+x)))/2.
 * https://oeis.org/A323619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} Stirling1(n,k)*A000272(k), with A000272(k) =
 *            k^(k-2) for k >= 2 and A000272(0) = A000272(1) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 11, u64 n <= 18, u128 n <= 29,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 395 (2026-11-08)
 * Helpers:   ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/ipow_idx.h"

static seq_t A000272_a(idx_t n)
{
    if (n <= 2)
        return 1;
    return ipow_idx((seq_t)n, n - 2);
}

seq_t a(idx_t n)
{
    idx_t m, k;
    seq_t *c = mem_new(seq_t, (size_t)n + 2);
    seq_t pos = 0, neg = 0, res;

    /* unsigned Stirling numbers of the first kind, row by row, in place */
    c[0] = 1;
    for (m = 0; m < n; m++) {
        for (k = m + 1; k >= 1; k--)
            c[k] = (seq_t)m * c[k] + c[k - 1];
        c[0] = (seq_t)m * c[0];
    }

    /* the sign of Stirling1(n,k) is (-1)^(n-k) */
    for (k = 0; k <= n; k++) {
        if (c[k] == 0)
            continue;
        seq_t term = c[k] * A000272_a(k);
        if (((n - k) & 1) == 0)
            pos += term;
        else
            neg += term;
    }
    res = pos - neg;
    mem_free(c);
    return res;
}
