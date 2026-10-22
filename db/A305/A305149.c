/*
 * A305149  Number of factorizations of n whose distinct factors are pairwise
 *          indivisible and greater than 1.
 * https://oeis.org/A305149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of factorizations of n into factors >1 whose
 *            distinct factors are pairwise indivisible. The algorithm
 *            enumerates all unordered factorizations of n in non‑decreasing
 *            order and checks the pairwise indivisibility incrementally,
 *            allowing repeated factors
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Recursive generation of all divisors of n */
static void gen_div(idx_t cur, const idx_t *pr, const idx_t *ex,
                    size_t pos, size_t cnt, idx_t *out, size_t *outcnt)
{
    if (pos == cnt) {
        if (cur >= 2) out[(*outcnt)++] = cur;
        return;
    }
    idx_t p = pr[pos];
    idx_t val = cur;
    for (size_t e = 0; e <= ex[pos]; ++e) {
        gen_div(val, pr, ex, pos + 1, cnt, out, outcnt);
        val *= p;
    }
}

static void get_divisors(idx_t n, idx_t *out, size_t *outcnt)
{
    if (n < 2) { *outcnt = 0; return; }
    idx_t pr[16], ex[16];
    size_t num;
    factor_u64(n, pr, ex, &num);
    size_t cnt = 0;
    gen_div(1, pr, ex, 0, num, out, &cnt);
    *outcnt = cnt;
}

/* Depth‑first search for factorizations.
   `start` guarantees non‑decreasing order. */
static void dfs(idx_t rem, idx_t start, idx_t *curr,
                int len, seq_t *count)
{
    if (rem == 1) {          /* complete factorization */
        (*count) += 1;
        return;
    }
    idx_t divs[128];
    size_t divcnt;
    get_divisors(rem, divs, &divcnt);
    for (size_t i = 0; i < divcnt; ++i) {
        idx_t d = divs[i];
        if (d < start) continue;
        /* check pairwise indivisibility with existing factors */
        bool ok = true;
        for (int j = 0; j < len; ++j) {
            idx_t f = curr[j];
            if (f != d && d % f == 0) { ok = false; break; }
        }
        if (!ok) continue;
        curr[len++] = d;
        dfs(rem / d, d, curr, len, count);
        --len;                /* backtrack */
    }
}

seq_t a(idx_t n)
{
    /* a(1) = 1 is handled by the recursion: dfs(1, 2) counts the empty
       factorization as the single valid factorization. */
    seq_t cnt = 0;
    idx_t curr[64];
    dfs(n, 2, curr, 0, &cnt);
    return cnt;
}
