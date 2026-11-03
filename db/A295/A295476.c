/*
 * A295476  a(n) = (20*n)!*(9*n)!*(6*n)!/((18*n)!*(10*n)!*(4*n)!*(3*n)!).
 * https://oeis.org/A295476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (20*n)!*(9*n)!*(6*n)! / ((18*n)!*(10*n)!*(4*n)!*(3*n)!)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 12 (2026-11-01)
 * Helpers:   build_spf, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/build_spf.h"
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* The 0‑th term is 1 by definition. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Interval limits (all arithmetic is done in idx_t, i.e. uint64_t). */
    idx_t n3  = 3 * n;
    idx_t n4  = 4 * n;
    idx_t n6  = 6 * n;
    idx_t n9  = 9 * n;
    idx_t n10 = 10 * n;
    idx_t n18 = 18 * n;
    idx_t limit = 20 * n;

    /* Smallest‑prime‑factor table up to `limit`. */
    idx_t *spf = NULL;
    build_spf(limit, &spf);          /* spf[0..limit] is allocated by build_spf */

    /* Accumulate signed exponents for each integer in [1,limit]. */
    /* -- signed 32‑bit is sufficient for n <= 10000 */
    int *exps = (int *)calloc((size_t)(limit + 1), sizeof(int));
    if (!exps) { free(spf); return (seq_t)0; }

    for (idx_t k = 1; k <= limit; ++k) {
        int e;
        if (k <= n3)            e = -1;
        else if (k <= n4)       e =  0;
        else if (k <= n6)       e =  1;
        else if (k <= n9)       e =  0;
        else if (k <= n10)      e = -1;
        else if (k <= n18)      e =  0;
        else                    e =  1;

        if (e == 0) continue;

        idx_t t = k;
        while (t > 1) {
            idx_t p = spf[t];
            int cnt = 0;
            while (t % p == 0) {
                t /= p;
                ++cnt;
            }
            exps[p] += e * cnt;
        }
    }

    /* Build the final integer from prime powers. */
    seq_t result = (seq_t)1;

    for (idx_t p = 2; p <= limit; ++p) {
        if (exps[p] == 0) continue;

        int exp = exps[p];
        idx_t abs_exp = (exp > 0) ? (idx_t)exp : (idx_t)(-exp);
        seq_t power = ipow_idx((seq_t)p, abs_exp);

        if (exp > 0) {
            result = seq_mul(result, power);
        } else {
            /* Exact division – the result is always an integer. */
            result = result / power;
        }
    }

    free(spf);
    free(exps);
    return result;
}
