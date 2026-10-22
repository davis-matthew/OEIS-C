/*
 * A375422  a(n) is the maximum number of points from the set {(k, prime(k)),
 *          k = 1..n} belonging to a straight line passing through the point
 *          (n, prime(n)) (where prime(k) denotes the k-th prime number).
 * https://oeis.org/A375422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the maximum number of points from {(k,prime(k)), k=1..n}
   that lie on a straight line passing through the point (n,prime(n)). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* cache of primes generated so far (1‑based indexing) */
static uint64_t *prime_cache   = NULL;
static size_t   prime_cache_sz = 0;

/* Euclidean GCD for 64‑bit signed integers */
static inline long long llgcd(long long a, long long b)
{
    while (b != 0) {
        long long t = a % b;
        a = b;
        b = t;
    }
    return a < 0 ? -a : a;
}

/* Ensure that prime_cache contains at least n primes.  The cached
   array uses 1‑based indexing: prime_cache[i-1] is the i‑th prime. */
static void ensure_primes(size_t n)
{
    if (n <= prime_cache_sz) return;

    size_t new_cap = prime_cache_sz ? prime_cache_sz * 2 : 32;
    while (new_cap < n) new_cap *= 2;

    uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
    if (!tmp) exit(1);              /* abort on allocation failure */
    prime_cache = tmp;

    uint64_t cand = prime_cache_sz ? prime_cache[prime_cache_sz - 1] + 1 : 2;
    while (prime_cache_sz < n) {
        bool is_prime = true;
        for (size_t j = 0; j < prime_cache_sz; ++j) {
            uint64_t p = prime_cache[j];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) prime_cache[prime_cache_sz++] = cand;
        ++cand;
    }
}

/* Normalised slope representation (dy,dx) with dx>0, gcd(dy,dx)=1 */
struct Slope {
    long long dy;
    long long dx;
};

static int slope_cmp(const void *a, const void *b)
{
    const struct Slope *p = (const struct Slope *)a;
    const struct Slope *q = (const struct Slope *)b;
    if (p->dy < q->dy) return -1;
    if (p->dy > q->dy) return 1;
    if (p->dx < q->dx) return -1;
    if (p->dx > q->dx) return 1;
    return 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined but safe */
    if (n == 1) return 1;          /* only point (1,2) */

    /* Generate primes up to the n-th prime */
    ensure_primes((size_t)n);
    uint64_t pn = prime_cache[n - 1];    /* prime(n) */

    size_t m = (size_t)n - 1;               /* number of points k < n */
    struct Slope *arr = (struct Slope *)malloc(m * sizeof(struct Slope));
    if (!arr) exit(1);

    for (size_t i = 0; i < m; ++i) {
        size_t k = i + 1;                   /* k in 1..n-1 */
        uint64_t pk = prime_cache[k - 1];
        long long dy = (long long)pn - (long long)pk;
        long long dx = (long long)n   - (long long)k;    /* positive */
        long long g  = llgcd(dy >= 0 ? dy : -dy, dx);
        if (dy == 0) {                     /* horizontal line */
            arr[i].dy = 0;
            arr[i].dx = 1;
        } else {
            arr[i].dy = dy / g;
            arr[i].dx = dx / g;
        }
    }

    /* Sort the slopes to find the most frequent one */
    qsort(arr, m, sizeof(struct Slope), slope_cmp);

    /* Scan through the sorted array, counting runs of equal slopes */
    size_t max_run = 1, cur_run = 1;
    for (size_t i = 1; i < m; ++i) {
        if (arr[i].dy == arr[i-1].dy && arr[i].dx == arr[i-1].dx) {
            ++cur_run;
        } else {
            if (cur_run > max_run) max_run = cur_run;
            cur_run = 1;
        }
    }
    if (cur_run > max_run) max_run = cur_run;

    free(arr);

    /* Include the point (n, prime(n)) itself */
    return (seq_t)(max_run + 1);
}
