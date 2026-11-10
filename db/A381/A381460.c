/*
 * A381460  Smallest n-th perfect power greater than 1 satisfying
 *          A373387(a(n)) = n.
 * https://oeis.org/A381460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, hard
 * Formula:   a(n) = min over b>=2 of b^n with A373387(b^n) = n (A373387 as in
 *            OEIS). Computed by: (i) 2-adic case 5|b, b = c*2^k +- 1 (c odd,
 *            k = n - v2(n) for even n, k = n for odd n), (ii) 5-adic case,
 *            Hensel digit-lifting of b mod 5^j for v5(f(b^n)) = n exactly,
 *            f(m) = m-1, m+1, m^2+1. Result is the smalle...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-10)
 * Helpers:   ipow_idx, mem, mulmod_seq, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"
#include "../utils/powmod.h"
#include "../utils/mulmod_seq.h"
#include "../utils/ipow_idx.h"

static seq_t A381460_resid(seq_t m, int t, seq_t M)
{
    if (t == 0) return (m + M - 1) % M;
    if (t == 1) return (m + 1) % M;
    return (mulmod_seq(m, m, M) + 1) % M;
}

/* smallest b >= 2 with v5(f_t(b^n)) == n exactly; returns true if found */
static bool A381460_search5(idx_t n, int t, seq_t *best)
{
    size_t cap = 16, cnt = 1, ncnt;
    seq_t *cur = mem_new(seq_t, cap);
    seq_t *nxt = mem_new(seq_t, cap);
    seq_t Mprev = 1, Mj;
    bool found = false;
    cur[0] = 0;
    for (idx_t j = 1; j <= n; j++)
    {
        Mj = Mprev * 5;
        ncnt = 0;
        for (size_t i = 0; i < cnt; i++)
        {
            for (int d = 0; d < 5; d++)
            {
                seq_t b = cur[i] + (seq_t)d * Mprev;
                seq_t m = powmod(b, (uint64_t)n, Mj);
                if (A381460_resid(m, t, Mj) == 0)
                {
                    if (ncnt >= cap)
                    {
                        cap *= 2;
                        cur = mem_resize(seq_t, cur, cap);
                        nxt = mem_resize(seq_t, nxt, cap);
                    }
                    nxt[ncnt++] = b;
                }
            }
        }
        seq_t *tmp = cur; cur = nxt; nxt = tmp;
        cnt = ncnt;
        Mprev = Mj;
    }
    {
        seq_t Mf = Mprev * 5;
        for (size_t i = 0; i < cnt; i++)
        {
            for (int d = 0; d < 5; d++)
            {
                seq_t b = cur[i] + (seq_t)d * Mprev;
                if (b < 2) continue;
                seq_t m = powmod(b, (uint64_t)n, Mf);
                if (A381460_resid(m, t, Mf) != 0)
                {
                    if (!found || b < *best) { *best = b; found = true; }
                }
            }
        }
    }
    mem_free(cur);
    mem_free(nxt);
    return found;
}

seq_t a(idx_t n)
{
    seq_t best = 0;
    bool found = false;

    /* 2-adic case: 5 | b */
    idx_t v2n = 0, tmp = n;
    while (tmp > 0 && (tmp & 1u) == 0u) { v2n++; tmp >>= 1; }
    idx_t k = (n & 1u) ? n : n - v2n;
    if (k >= 2)
    {
        seq_t P = 1;
        for (idx_t i = 0; i < k; i++) P = P * 2;
        for (unsigned c = 1; c < 60; c += 2)
        {
            seq_t x = (seq_t)c * P;
            if ((x - 1) % 5 == 0) { best = x - 1; found = true; break; }
            if ((x + 1) % 5 == 0) { best = x + 1; found = true; break; }
        }
    }

    /* 5-adic cases */
    for (int t = 0; t < 3; t++)
    {
        seq_t b = 0;
        if (A381460_search5(n, t, &b))
        {
            if (!found || b < best) { best = b; found = true; }
        }
    }

    return ipow_idx(best, n);
}
