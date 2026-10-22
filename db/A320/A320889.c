/*
 * A320889  Number of set partitions of strict factorizations of n into
 *          factors > 1 such that all the blocks have the same product.
 * https://oeis.org/A320889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by enumerating all strict factorizations of n *
 *            into factors > 1 and counting all unordered set partitions of
 *            the factor * multiset where every block has the same product.
 *            For a factorization * to allow such a partition the block
 *            product must be a divisor d>1 of n * satisfying d^...
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef MAX_DIVISORS
#define MAX_DIVISORS 256
#endif
#ifndef MAX_ROOTS
#define MAX_ROOTS 256
#endif
#ifndef MAX_FACTORS
#define MAX_FACTORS 20
#endif
#ifndef MAX_MASK
#define MAX_MASK (1 << 15)   /* 32768, enough for m ≤ 14 */
#endif

/* Context for a single n */
typedef struct {
    idx_t n;
    size_t div_cnt;                     /* number of divisors >1 of n          */
    uint64_t divs[MAX_DIVISORS];        /* sorted list of such divisors        */
    size_t root_cnt;                    /* number of d with d^k = n            */
    uint64_t roots[MAX_ROOTS];          /* the corresponding d values          */
    uint64_t prod[MAX_MASK];            /* products of chosen factors for submask */
    unsigned long long dp[MAX_MASK];    /* memoised partition count for mask   */
    unsigned char seen[MAX_MASK];       /* flags for memoisation               */
    uint64_t chosen[MAX_FACTORS];       /* current factorization being built   */
    unsigned long long total;           /* running total for the n             */
} SeqContext;

/* Count partitions of mask where each block product equals root */
static unsigned long long dfs_count(uint32_t mask, uint64_t root,
                                   uint64_t *prod, unsigned long long *dp,
                                   unsigned char *seen)
{
    if (mask == 0) return 1;
    if (seen[mask]) return dp[mask];
    unsigned long long ans = 0;
    int i = __builtin_ctz(mask);            /* ensure canonical ordering       */
    for (uint32_t sub = mask; sub; sub = (sub - 1) & mask) {
        if (!(sub & (1u << i))) continue;   /* sub must contain i              */
        if (prod[sub] == root) {
            ans += dfs_count(mask ^ sub, root, prod, dp, seen);
        }
    }
    seen[mask] = 1;
    dp[mask] = ans;
    return ans;
}

/* Recursively build factorizations of rem into factors >1   */
static void dfs_factor(SeqContext *ctx, size_t start_idx,
                       uint64_t rem, size_t depth)
{
    if (rem == 1) {
        size_t m = depth;
        uint32_t M = 1u << m;
        ctx->prod[0] = 1;
        for (uint32_t mask = 1; mask < M; ++mask) {
            int lsb = __builtin_ctz(mask);
            ctx->prod[mask] = ctx->prod[mask ^ (1u << lsb)] *
                               (uint64_t)ctx->chosen[lsb];
        }
        for (size_t r = 0; r < ctx->root_cnt; ++r) {
            uint64_t root_val = ctx->roots[r];
            memset(ctx->seen, 0, M);
            ctx->total += dfs_count(M - 1, root_val,
                                    ctx->prod, ctx->dp, ctx->seen);
        }
        return;
    }
    for (size_t i = start_idx; i < ctx->div_cnt; ++i) {
        uint64_t d = ctx->divs[i];
        if (d > rem) break;
        if (rem % d != 0) continue;
        ctx->chosen[depth] = d;
        dfs_factor(ctx, i + 1, rem / d, depth + 1);
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    SeqContext ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.n = n;

    /* collect all divisors > 1 of n, sorted increasingly */
    size_t div_cnt = 0;
    for (uint64_t d = 2; d * d <= (uint64_t)n; ++d) {
        if (n % d == 0) {
            ctx.divs[div_cnt++] = d;
            if (d * d != (uint64_t)n) ctx.divs[div_cnt++] = n / d;
        }
    }
    ctx.divs[div_cnt++] = (uint64_t)n;   /* include n itself */
    ctx.div_cnt = div_cnt;

    /* simple insertion sort */
    for (size_t i = 0; i < ctx.div_cnt; ++i)
        for (size_t j = i + 1; j < ctx.div_cnt; ++j)
            if (ctx.divs[i] > ctx.divs[j]) {
                uint64_t tmp = ctx.divs[i];
                ctx.divs[i] = ctx.divs[j];
                ctx.divs[j] = tmp;
            }

    /* compute all divisor bases d such that d^k = n */
    for (size_t i = 0; i < ctx.div_cnt; ++i) {
        uint64_t d = ctx.divs[i];
        uint64_t p = d;
        while (p < (uint64_t)n) p *= d;
        if (p == (uint64_t)n) {
            bool dup = false;
            for (size_t j = 0; j < ctx.root_cnt; ++j)
                if (ctx.roots[j] == d) { dup = true; break; }
            if (!dup) ctx.roots[ctx.root_cnt++] = d;
        }
    }

    ctx.total = 0;
    dfs_factor(&ctx, 0, (uint64_t)n, 0);
    return (seq_t)ctx.total;
}
