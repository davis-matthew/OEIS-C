/*
 * A347458  Number of factorizations of n^2 with integer alternating product.
 * https://oeis.org/A347458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of weakly increasing factorizations of n^2 into
 *            integers >1 such that the alternating product of the factors is
 *            an integer
 * Range:     u8 n <= 119, u16 n <= 7559, u32 n <= 9805, u64 n <= 9805,
 *            u128 n <= 9805, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* maximum number of distinct primes for n <= 10000 (5 primes, 2*3*5*7*11=2310; 2*3*5*7*11*13=30030>10000) */
#define MAXP 6
/* maximum number of divisors for n^2 <= 1e8 (fits into 2000) */
#define MAXD 2000

typedef struct {
    uint32_t value;
    int exps[MAXP];
} Div;

typedef struct {
    const Div *divs;   /* array of divisors of n^2             */
    size_t div_cnt;    /* number of divisors                  */
    int        pcnt;   /* number of distinct primes           */
    int        diff[MAXP]; /* accumulated exponent difference odd-even */
    seq_t      count;   /* result                                 */
} Context;

static int cmp_uint32(const void *a, const void *b)
{
    uint32_t va = ((const Div *)a)->value;
    uint32_t vb = ((const Div *)b)->value;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* build all divisors of n^2 */
static void gen_divisors_rec(int idx, const int primes[MAXP], const int exponents[MAXP],
                            int pcnt, uint64_t cur, int exps_tmp[MAXP],
                            Div *divs, size_t *div_cnt)
{
    if (idx == pcnt) {
        Div d;
        d.value = (uint32_t)cur;
        memcpy(d.exps, exps_tmp, pcnt * sizeof(int));
        divs[(*div_cnt)++] = d;
        return;
    }
    int p = primes[idx];
    int emax = exponents[idx];
    uint64_t val = cur;
    for (int k = 0; k <= emax; ++k) {
        exps_tmp[idx] = k;
        gen_divisors_rec(idx + 1, primes, exponents, pcnt, val, exps_tmp, divs, div_cnt);
        if (k == emax) break;
        val *= (uint64_t)p;
    }
}

static void build_divisors(const int primes[MAXP], const int exponents[MAXP],
                           int pcnt, Div *divs, size_t *div_cnt)
{
    int exps_tmp[MAXP];
    gen_divisors_rec(0, primes, exponents, pcnt, 1ULL, exps_tmp, divs, div_cnt);
    /* sort by divisor value */
    qsort(divs, *div_cnt, sizeof(Div), cmp_uint32);
}

/* recursive enumeration of factorizations */
static void dfs(Context *ctx, size_t start_idx, uint64_t rem, int parity)
{
    if (rem == 1ULL) {
        for (int p = 0; p < ctx->pcnt; ++p) {
            if (ctx->diff[p] < 0 || (ctx->diff[p] & 1))
                return;  /* not integer alternating product */
        }
        ++ctx->count;
        return;
    }
    for (size_t i = start_idx; i < ctx->div_cnt; ++i) {
        uint32_t f = ctx->divs[i].value;
        if (f == 1U) continue;
        if (f > rem) break;
        if (rem % f != 0ULL) continue;
        /* apply exponent changes */
        for (int p = 0; p < ctx->pcnt; ++p) {
            int delta = parity ? -ctx->divs[i].exps[p] : ctx->divs[i].exps[p];
            ctx->diff[p] += delta;
        }
        dfs(ctx, i, rem / f, parity ^ 1);
        /* revert */
        for (int p = 0; p < ctx->pcnt; ++p) {
            int delta = parity ? -ctx->divs[i].exps[p] : ctx->divs[i].exps[p];
            ctx->diff[p] -= delta;
        }
    }
}

/* helper to factor n into primes */
static void factor_uint64(uint64_t n, int primes[MAXP], int exponents[MAXP], int *pcnt)
{
    *pcnt = 0;
    uint64_t x = n;
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0ULL) {
            int e = 0;
            while (x % p == 0ULL) {
                x /= p;
                ++e;
            }
            primes[*pcnt] = (int)p;
            exponents[*pcnt] = e;
            ++(*pcnt);
        }
    }
    if (x > 1) {
        if (*pcnt < MAXP) {
            primes[*pcnt] = (int)x;
            exponents[*pcnt] = 1;
            ++(*pcnt);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 1ULL) return (seq_t)1;
    int primes[MAXP], exponents[MAXP];
    int pcnt;
    factor_uint64((uint64_t)n, primes, exponents, &pcnt);
    /* square the exponents for n^2 */
    for (int i = 0; i < pcnt; ++i) exponents[i] *= 2;
    /* build divisors of n^2 */
    Div divs[MAXD];
    size_t div_cnt = 0;
    build_divisors(primes, exponents, pcnt, divs, &div_cnt);
    /* context */
    Context ctx;
    ctx.divs  = divs;
    ctx.div_cnt = div_cnt;
    ctx.pcnt  = pcnt;
    memset(ctx.diff, 0, sizeof(ctx.diff));
    ctx.count = (seq_t)0;
    dfs(&ctx, 0, (uint64_t)n * (uint64_t)n, 0);
    return ctx.count;
}
