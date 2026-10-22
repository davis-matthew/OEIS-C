/*
 * A305079  Number of connected components of the integer partition with Heinz
 *          number n.
 * https://oeis.org/A305079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8749, u16 n <= 8749, u32 n <= 8749, u64 n <= 8749,
 *            u128 n <= 8749, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): number of connected components of the graph G(S)
   for the integer partition whose Heinz number is n. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* -----------------------------------------------------------------------
   Prime table (shared for all calls)
   ----------------------------------------------------------------------- */
#define PRIME_LIMIT 200000          /* sufficient for n ≤ 10000 */

static uint32_t *prime_list = NULL;/* array of prime numbers (1‑based) */
static size_t   prime_cnt  = 0;    /* number of primes stored */

/* Build the prime list lazily.  The array is allocated only once. */
static void build_prime_list(void)
{
    if (prime_list != NULL) return;          /* already built */

    bool *is_prime = (bool*)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_prime) abort();
    for (int i = 2; i <= PRIME_LIMIT; ++i) is_prime[i] = true;

    for (int i = 2; (long long)i * i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = false;

    prime_list = (uint32_t*)calloc(PRIME_LIMIT / 2 + 10, sizeof(uint32_t));
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) prime_list[prime_cnt++] = (uint32_t)i;

    free(is_prime);
}

/* Return the 1‑based index of a prime p.
   p must be prime and ≤ PRIME_LIMIT; otherwise 0 is returned. */
static idx_t prime_index(uint64_t p)
{
    build_prime_list();
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] == (uint32_t)p) return mid + 1;
        if (prime_list[mid] < (uint32_t)p) lo = mid + 1;
        else hi = mid;
    }
    return 0;   /* p not found – should not happen for valid input */
}

/* -----------------------------------------------------------------------
   Euclidean GCD for unsigned 64‑bit indices
   ----------------------------------------------------------------------- */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b; b = t;
    }
    return a;
}

/* -----------------------------------------------------------------------
   Trial division factorization of a 64‑bit integer
   ----------------------------------------------------------------------- */
static size_t factor_u64(uint64_t n, uint64_t primes[], int exps[])
{
    if (n < 2) return 0;
    build_prime_list();
    size_t cnt = 0;
    uint64_t t = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= t; ++i) {
        uint64_t p = prime_list[i];
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt++] = e;
        }
    }
    if (t > 1) {               /* remaining prime factor */
        primes[cnt] = t;
        exps[cnt] = 1;
        ++cnt;
    }
    return cnt;
}

/* -----------------------------------------------------------------------
   Union–Find structure for small number of vertices (≤ 64)
   ----------------------------------------------------------------------- */
typedef struct {
    size_t n;   /* number of elements */
    size_t *p;  /* parent array */
    size_t *r;  /* rank array */
    size_t comp;/* number of components */
} dsu_t;

static dsu_t *dsu_new(size_t n)
{
    dsu_t *d = (dsu_t*)malloc(sizeof(dsu_t));
    d->n = n;
    d->p = (size_t*)malloc(n * sizeof(size_t));
    d->r = (size_t*)malloc(n * sizeof(size_t));
    for (size_t i = 0; i < n; ++i) { d->p[i] = i; d->r[i] = 0; }
    d->comp = n;
    return d;
}

static void dsu_free(dsu_t *d)
{
    free(d->p); free(d->r); free(d);
}

static size_t dsu_find(dsu_t *d, size_t x)
{
    if (d->p[x] != x) d->p[x] = dsu_find(d, d->p[x]);
    return d->p[x];
}

static void dsu_union(dsu_t *d, size_t a, size_t b)
{
    a = dsu_find(d, a);  b = dsu_find(d, b);
    if (a == b) return;
    if (d->r[a] < d->r[b]) d->p[a] = b;
    else if (d->r[a] > d->r[b]) d->p[b] = a;
    else { d->p[b] = a; ++d->r[a]; }
    --d->comp;
}

/* -----------------------------------------------------------------------
   Main sequence function
   ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Special case n = 1 (empty partition) */
    if (n == 1) return (seq_t)0;

    /* Factorize n */
    uint64_t primes[64];
    int exps[64];
    size_t num = factor_u64(n, primes, exps);

    /* Separate parts into indices > 1 and the count of parts 1 (i.e. part 1) */
    size_t part_cnt = 0;
    idx_t parts[64];
    size_t ones = 0;                   /* number of part‑value 1 (indices of prime 2) */

    for (size_t i = 0; i < num; ++i) {
        uint64_t p = primes[i];
        int e   = exps[i];
        idx_t idx = prime_index(p);
        if (idx == 0) continue;        /* safety – should not happen */
        if (idx == 1)                 /* part value 1 (prime 2) */
            ones += (size_t)e;
        else
            parts[part_cnt++] = idx;
    }

    /* Count connected components among the unique part values > 1 */
    size_t comps = 0;
    if (part_cnt > 0) {
        dsu_t *d = dsu_new(part_cnt);

        for (size_t i = 0; i < part_cnt; ++i)
            for (size_t j = i + 1; j < part_cnt; ++j)
                if (gcd_idx(parts[i], parts[j]) > 1)
                    dsu_union(d, i, j);

        comps = d->comp;
        dsu_free(d);
    }

    /* The 1's each form an isolated component */
    return (seq_t)(ones + comps);
}
