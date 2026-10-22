/*
 * A337736  The number of cubefull numbers (A036966) between the consecutive
 *          cubes n^3 and (n+1)^3.
 * https://oeis.org/A337736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 42, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of cubefull numbers (each prime exponent ≥ 3)
 * between the consecutive cubes n^3 and (n+1)^3.
 * The function pre‑computes all cubefull numbers up to a fixed limit
 * (1 ≤ n ≤ 20000) once and answers queries by binary search.
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* -----------------  precomputation  ----------------- */
static size_t init_done = 0;
static idx_t   MAX_N = 20000;                  /* upper bound safe for tests      */
static seq_t   LIMIT;                          /* (MAX_N+1)^3                     */
static seq_t  *cubefull_vec   = NULL;          /* array of cubefull numbers       */
static size_t cubefull_len   = 0;              /* number of elements              */
static size_t cubefull_cap   = 0;              /* current capacity                */
static uint32_t *spf           = NULL;          /* smallest prime factor array     */
static size_t  spf_size   = 0;                  /* size of spf array               */

/* allocate and fill spf array */
static void build_spf(size_t limit)
{
    spf_size = limit + 1;
    spf = (uint32_t *)malloc(spf_size * sizeof(uint32_t));
    for (size_t i = 0; i < spf_size; ++i) spf[i] = 0;
    for (uint32_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if ((uint64_t)i * i <= limit) {
                for (uint32_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
}

/* helper to push a value into the dynamic array */
static void push_val(seq_t val)
{
    if (cubefull_len == cubefull_cap) {
        cubefull_cap = cubefull_cap ? cubefull_cap * 2 : 1024;
        seq_t *tmp = (seq_t *)realloc(cubefull_vec, cubefull_cap * sizeof(seq_t));
        if (!tmp) { cubefull_vec = NULL; return; }
        cubefull_vec = tmp;
    }
    cubefull_vec[cubefull_len++] = val;
}

/* recursive generation of divisors of c^2 */
static void gen_div(const int *primes, const int *exps, size_t idx, size_t pcnt,
                    uint64_t cur, uint64_t cube, uint64_t limit)
{
    if (idx == pcnt) {
        unsigned __int128 prod = (unsigned __int128)cube * cur;
        if (prod <= limit) {
            push_val((seq_t)(uint64_t)prod);
        }
        return;
    }
    uint64_t p = primes[idx];
    int maxf = exps[idx] * 2;
    uint64_t pw = 1;
    for (int f = 0; f <= maxf; ++f) {
        gen_div(primes, exps, idx + 1, pcnt, cur * pw, cube, limit);
        pw *= p;
    }
}

/* comparison for qsort */
static int cmp_seq(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* initialise static data once */
static void init(void)
{
    if (init_done) return;
    init_done = 1;

    /* compute LIMIT = (MAX_N+1)^3 */
    uint64_t mx = (uint64_t)(MAX_N + 1);
    LIMIT = (seq_t)(mx * mx * mx);

    /* build prime factor sieve up to MAX_N+1 */
    build_spf(MAX_N + 1);

    /* generate cubefull numbers */
    cubefull_cap = 1024;
    cubefull_vec = (seq_t *)malloc(cubefull_cap * sizeof(seq_t));

    int primes[32], exps[32];
    const size_t MAX_PR = 32;

    for (uint32_t c = 1; c <= MAX_N + 1; ++c) {
        uint64_t cube = (uint64_t)c * c * c;
        if (cube > (uint64_t)(mx * mx * mx)) break;

        /* factorise c */
        uint32_t x = c;
        size_t pcnt = 0;
        while (x > 1) {
            uint32_t p = spf[x];
            uint32_t e = 0;
            while (x % p == 0) { x /= p; ++e; }
            primes[pcnt] = (int)p;
            exps[pcnt]  = (int)e;
            ++pcnt;
        }

        gen_div(primes, exps, 0, pcnt, 1ULL, cube, (uint64_t)(mx * mx * mx));
    }

    /* sort and deduplicate */
    qsort(cubefull_vec, cubefull_len, sizeof(seq_t), cmp_seq);
    size_t uniq = 0;
    for (size_t i = 0; i < cubefull_len; ++i) {
        if (uniq == 0 || cubefull_vec[i] != cubefull_vec[uniq - 1]) {
            cubefull_vec[uniq++] = cubefull_vec[i];
        }
    }
    cubefull_len = uniq;
}

/* upper_bound: first index > value */
static size_t upper_bound(const seq_t *arr, size_t len, seq_t value)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (arr[mid] <= value) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

/* -----------------  public API  ----------------- */
seq_t a(idx_t n)
{
    init();             /* ensure precomputation done */
    uint64_t n3    = (uint64_t)n * n * n;
    uint64_t np3   = (uint64_t)(n + 1) * (n + 1) * (n + 1);

    seq_t idx_upper = (seq_t)upper_bound(cubefull_vec, cubefull_len, (seq_t)(np3 - 1));
    seq_t idx_lower = (seq_t)upper_bound(cubefull_vec, cubefull_len, (seq_t)n3);

    return idx_upper - idx_lower;
}
