/*
 * A379523  Sum of coreful divisors d | k such that gcd(d, k/d) > 1 and rad(d)
 *          = rad(k/d), with d | k/d and d < d/k, where k is in A320966 and
 *          rad = A007947.
 * https://oeis.org/A379523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|k, d<k/d, gcd(d,k/d)>1, rad(d)=rad(k/d)} (d+k/d),
 *            where k = A320966(n). k is the n‑th number that is square‑full,
 *            has at least one prime exponent ≥ 3, and the sequence is sorted
 *            in increasing order
 * Range:     u8 n <= 22, u16 n <= 489, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Notes:
   • All arithmetic on values that fit in uint64_t uses native types.
   • seq_t is used only for the final sum; all intermediate
     calculations use uint64_t to avoid overflow.
   • The code is fully portable to C11 and C++17.  C++ compilers
     require an explicit cast from void* returned by realloc. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ----------------------------------------------------------------- */
/* -----------------    Prime sieve (up to 70 000)   ----------------- */
#define PRIME_LIMIT 70000                  /* enough for sqrt(4.0e9) */

static uint64_t *primes = NULL;            /* dynamic array of primes */
static size_t prime_count = 0;             /* number of primes stored */

static void init_primes(void)
{
    bool comp[PRIME_LIMIT + 1];
    memset(comp, 0, sizeof(comp));

    size_t alloc = PRIME_LIMIT / 10 + 10;
    primes = (uint64_t*)malloc(alloc * sizeof(uint64_t));
    if (!primes) abort();

    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!comp[i]) {
            if (prime_count == alloc) {    /* grow array */
                alloc *= 2;
                uint64_t *tmp = (uint64_t*)realloc(primes, alloc * sizeof(uint64_t));
                if (!tmp) abort();
                primes = tmp;
            }
            primes[prime_count++] = i;
            for (uint64_t j = i * i; j <= PRIME_LIMIT; j += i)
                comp[j] = true;
        }
    }
}

/* ----------------------------------------------------------------- */
/* -----------------   Square‑full numbers generator  ----------------- */
#define SQ_LIMIT 4000000000ULL               /* 4·10⁹ – enough for the first 10 000 terms */

static uint64_t *sq_nums   = NULL;           /* dynamic array of square‑full numbers */
static size_t     sq_cnt   = 0;              /* number of generated numbers */

/* push a number into the array, growing as necessary */
static void push_sq(uint64_t val)
{
    if (sq_cnt % 10000 == 0) {              /* grow in blocks of 10 000 */
        size_t newsize = sq_cnt + 10000;
        uint64_t *tmp = (uint64_t*)realloc(sq_nums, newsize * sizeof(uint64_t));
        if (!tmp) abort();
        sq_nums = tmp;
    }
    sq_nums[sq_cnt++] = val;
}

/* recursive generation of square‑full numbers with at least one
   exponent ≥ 3 */
static void gen_sq(uint64_t cur, bool has_ge3, size_t start_idx)
{
    if (cur > SQ_LIMIT) return;

    if (cur > 1 && has_ge3)
        push_sq(cur);

    for (size_t i = start_idx; i < prime_count; ++i) {
        uint64_t p = primes[i];
        uint64_t p2 = p * p;
        if (cur > SQ_LIMIT / p2) break;          /* further primes would exceed limit */

        uint64_t p_pow = p2;
        int exp = 2;
        bool ge3 = has_ge3;

        while (cur <= SQ_LIMIT / p_pow) {
            if (exp >= 3) ge3 = true;
            gen_sq(cur * p_pow, ge3, i + 1);
            ++exp;
            if (exp >= 63) break;                /* prevent overflow of exp */
            if (p_pow > SQ_LIMIT / p) break;
            p_pow *= p;
        }
    }
}

/* ----------------------------------------------------------------- */
/* -----------------  S(n) = A320966(n)  ---------------------------- */
static uint64_t s_table[10001];   /* 1‑based index */
static bool s_ready = false;

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t av = *(const uint64_t*)a;
    uint64_t bv = *(const uint64_t*)b;
    return (av < bv) ? -1 : (av > bv) ? 1 : 0;
}

static void build_s_table(void)
{
    init_primes();

    sq_nums = NULL;
    sq_cnt = 0;
    gen_sq(1ULL, false, 0);

    if (sq_cnt < 10000) abort();                 /* not enough numbers */

    qsort(sq_nums, sq_cnt, sizeof(uint64_t), cmp_uint64);

    /* first 10 000 entries form A320966 */
    for (int i = 0; i < 10000; ++i)
        s_table[i + 1] = sq_nums[i];

    s_ready = true;
}

/* ----------------------------------------------------------------- */
/* -----------------  Factorisation of k  --------------------------- */
static void factorise(uint64_t n, uint64_t primes_out[16], int exps_out[16], int *cnt)
{
    *cnt = 0;
    for (size_t i = 0; i < prime_count && primes[i] * primes[i] <= n; ++i) {
        if (n % primes[i] == 0) {
            int e = 0;
            while (n % primes[i] == 0) {
                n /= primes[i];
                ++e;
            }
            primes_out[(*cnt)] = primes[i];
            exps_out[(*cnt)]   = e;
            (*cnt)++;
        }
    }
    if (n > 1) {
        primes_out[(*cnt)] = n;
        exps_out[(*cnt)]   = 1;
        (*cnt)++;
    }
}

/* ----------------------------------------------------------------- */
/* -----------------  DFS over coreful divisor pairs  ---------------- */
static void dfs_div(uint64_t k,
                    const uint64_t *pr,
                    const int    *ex,
                    int           pos,
                    uint64_t      curr_d,
                    bool          all_half,
                    seq_t        *sum,
                    int           total)
{
    if (pos == total) {
        if (all_half) return;    /* skip d = k/d (equal pair) */
        seq_t d = curr_d;
        seq_t other = (seq_t)(k / curr_d);
        *sum += d + other;
        return;
    }

    int e   = ex[pos];
    int half = e / 2;
    bool even = (e % 2 == 0);

    for (int f = 1; f <= half; ++f) {
        uint64_t p_pow = 1;
        for (int i = 0; i < f; ++i) p_pow *= pr[pos];
        uint64_t new_d = curr_d * p_pow;
        bool new_all_half = all_half && (even && f == half);
        dfs_div(k, pr, ex, pos + 1, new_d, new_all_half, sum, total);
    }
}

/* ----------------------------------------------------------------- */
/* -----------------  Main sequence function  ------------------------ */
seq_t a(idx_t n)
{
    if (!s_ready) build_s_table();

    uint64_t k = s_table[(size_t)n];

    /* factorise k */
    uint64_t  prms[16];
    int       exps[16];
    int       cnt;
    factorise(k, prms, exps, &cnt);

    /* compute sum of the required divisor pairs */
    seq_t sum = 0;
    dfs_div(k, prms, exps, 0, 1ULL, true, &sum, cnt);

    return sum;
}
