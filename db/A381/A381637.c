/*
 * A381637  Number of multisets that can be obtained by taking the sum of each
 *          block of a multiset partition of the prime indices of n into
 *          blocks with distinct sums.
 * https://oeis.org/A381637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A381637: a(n) = number of distinct multisets of sums obtained from
   partitions of the multiset of prime indices of n into blocks with
   distinct sums. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------ */
/* Helper data for primes                                         */
#define MAX_PRIMES 2000
static int prime_list[MAX_PRIMES];
static int prime_cnt = 0;
static bool primes_initialised = false;

static void generate_primes(unsigned limit)
{
    bool is_prime[limit + 1];
    for (unsigned i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (unsigned i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (unsigned j = i * i; j <= limit; j += i) is_prime[j] = false;
        }
    }
    int idx = 0;
    for (unsigned i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            prime_list[idx++] = (int)i;
        }
    }
    prime_cnt = idx;
}

static int prime_index(uint64_t p)
{
    for (int i = 0; i < prime_cnt; ++i) {
        if (prime_list[i] == (int)p) return i + 1;     /* 1‑based index */
    }
    return -1;
}

/* ------------------------------------------------------------ */
/* Context for the DFS                                         */
#define MAX_K 20
typedef struct {
    int values[MAX_K];        /* distinct prime‑index values */
    int counts[MAX_K];        /* remaining multiplicities   */
    int m;                    /* number of distinct values  */
    int curr_sums[MAX_K];     /* sums of blocks built so far */
    int curr_len;             /* number of sums in current recursion */
    char **results;           /* list of unique result strings */
    int res_len;              /* number of unique results */
    int res_cap;              /* capacity of results array */
} Context;

static void add_result(Context *ctx)
{
    char buffer[256];
    int pos = 0;
    for (int i = 0; i < ctx->curr_len; ++i) {
        pos += sprintf(buffer + pos, "%d,", ctx->curr_sums[i]);
    }
    for (int i = 0; i < ctx->res_len; ++i) {
        if (strcmp(buffer, ctx->results[i]) == 0) {   /* duplicate */
            return;
        }
    }
    if (ctx->res_len == ctx->res_cap) {
        ctx->res_cap <<= 1;
        ctx->results = (char **)realloc(ctx->results,
                                         ctx->res_cap * sizeof(char *));
    }
    ctx->results[ctx->res_len] = (char *)malloc(pos + 1);
    memcpy(ctx->results[ctx->res_len], buffer, pos + 1);
    ctx->res_len++;
}

/* Forward declaration of DFS for mutual recursion */
static void dfs(Context *ctx, int min_sum);

/* Enumerate all possible sub‑multisets of the current state */
static void enumerate_subsets(int idx, int cur_sum, int *take,
                              Context *ctx, int min_sum)
{
    if (idx == ctx->m) {
        if (cur_sum > min_sum) {
            int any = 0;
            for (int j = 0; j < ctx->m; ++j) {
                if (take[j] > 0) { any = 1; break; }
            }
            if (!any) return;            /* empty subset – skip */
            for (int j = 0; j < ctx->m; ++j) ctx->counts[j] -= take[j];
            ctx->curr_sums[ctx->curr_len++] = cur_sum;
            dfs(ctx, cur_sum);            /* next block must have sum > cur_sum */
            ctx->curr_len--;
            for (int j = 0; j < ctx->m; ++j) ctx->counts[j] += take[j];
        }
        return;
    }
    int max_take = ctx->counts[idx];
    for (int t = 0; t <= max_take; ++t) {
        take[idx] = t;
        enumerate_subsets(idx + 1, cur_sum + t * ctx->values[idx],
                          take, ctx, min_sum);
    }
}

static void dfs(Context *ctx, int min_sum)
{
    /* check if all counts are zero */
    int all_zero = 1;
    for (int i = 0; i < ctx->m; ++i) {
        if (ctx->counts[i] > 0) { all_zero = 0; break; }
    }
    if (all_zero) {                              /* a valid partition reached */
        add_result(ctx);
        return;
    }
    int take[MAX_K] = {0};
    enumerate_subsets(0, 0, take, ctx, min_sum);
}

/* ------------------------------------------------------------ */
/* Main sequence function                                         */
seq_t a(idx_t n)
{
    /* initialise primes once */
    if (!primes_initialised) {
        generate_primes(10000);
        primes_initialised = true;
    }

    if (n == 1) return (seq_t)1;

    /* factorise n */
    uint64_t tmp = n;
    int fac[32];
    int exp[32];
    int nf = 0;
    for (int i = 0; i < prime_cnt; ++i) {
        uint64_t p = (uint64_t)prime_list[i];
        if (p * p > tmp) break;
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            fac[nf] = (int)p;
            exp[nf] = e;
            ++nf;
        }
    }
    if (tmp > 1) {
        fac[nf] = (int)tmp;
        exp[nf] = 1;
        ++nf;
    }

    /* build multiset of prime indices */
    int mult[32];
    int mlen = 0;
    for (int i = 0; i < nf; ++i) {
        int idx = prime_index(fac[i]);           /* 1‑based index */
        for (int j = 0; j < exp[i]; ++j) {
            mult[mlen++] = idx;
        }
    }
    if (mlen == 0) return (seq_t)1;   /* only possible for n=1 */

    /* sort multiset */
    for (int i = 1; i < mlen; ++i) {
        int v = mult[i];
        int j = i - 1;
        while (j >= 0 && mult[j] > v) { mult[j + 1] = mult[j]; --j; }
        mult[j + 1] = v;
    }

    /* compress into values and counts */
    int values[MAX_K];
    int counts[MAX_K];
    int m = 0;
    for (int i = 0; i < mlen; ++i) {
        if (i == 0 || mult[i] != mult[i - 1]) {
            values[m] = mult[i];
            counts[m] = 1;
            ++m;
        } else {
            ++counts[m - 1];
        }
    }

    /* set up context */
    Context ctx;
    memcpy(ctx.values, values, m * sizeof(int));
    memcpy(ctx.counts, counts, m * sizeof(int));
    ctx.m = m;
    ctx.curr_len = 0;
    ctx.res_cap = 8;
    ctx.results = (char **)malloc(ctx.res_cap * sizeof(char *));
    ctx.res_len = 0;

    /* start DFS */
    dfs(&ctx, 0);

    /* cleanup results strings */
    for (int i = 0; i < ctx.res_len; ++i) free(ctx.results[i]);
    free(ctx.results);

    return (seq_t)ctx.res_len;
}
