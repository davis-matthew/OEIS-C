/*
 * A376497  a(n) is the maximum number of points from the set {(prime(k),
 *          prime(k+1)), k = 1..n} belonging to a straight line passing
 *          through the point (prime(n), prime(n+1)) (where prime(k) denotes
 *          the k-th prime number).
 * https://oeis.org/A376497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1052, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_int64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A376497:
 * a(n) is the maximum number of points from
 * {(prime(k),prime(k+1)), k=1..n} that are collinear with the point
 * (prime(n),prime(n+1)).
 *
 * The implementation generates the first (n+1) primes by trial division
 * (caching results for subsequent calls), then evaluates all slopes
 * from the target point to earlier points, normalizes them to reduced
 * fractions, sorts them and finds the largest equal‑slope run.
 * Complexity: O(n log n) per call for n up to ~10⁴.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd_int64.h"

/* ---------- prime cache ---------- */
static uint64_t *prime_cache   = NULL;   /* array of primes (0‑based) */
static size_t   prime_cap     = 0;       /* allocated capacity      */
static size_t   prime_cnt     = 0;       /* number of primes stored */
static uint64_t max_checked   = 1;       /* last integer tested    */

/* Ensure that prime_cache holds at least idx primes (idx ≥ 1). */
static void ensure_primes_by_index(idx_t idx)
{
    if (prime_cnt >= (size_t)idx) return;

    /* Grow the cache if needed. */
    if (prime_cap < (size_t)idx) {
        size_t new_cap = prime_cap > 0 ? prime_cap << 1 : 16;
        while (new_cap < (size_t)idx) new_cap <<= 1;
        uint64_t *tmp = (uint64_t*)realloc(prime_cache, new_cap * sizeof(uint64_t));
        if (!tmp) return;                   /* memory failure → garbage value */
        prime_cache = tmp;
        prime_cap   = new_cap;
    }

    /* Trial‑division to generate subsequent primes. */
    for (uint64_t cand = max_checked + 1;
         prime_cnt < (size_t)idx; ++cand) {
        bool isprime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_cache[i];
            if (p * p > cand) break;
            if (cand % p == 0) { isprime = false; break; }
        }
        if (isprime) prime_cache[prime_cnt++] = cand;
    }
    max_checked = prime_cache[prime_cnt - 1];
}

/* Representation of a reduced slope (numerator/denominator). */
typedef struct { int64_t num; int64_t den; } slope_t;

/* Comparator for qsort: first numerator, then denominator. */
static int cmp_slope(const void *a, const void *b)
{
    const slope_t *x = (const slope_t*)a;
    const slope_t *y = (const slope_t*)b;
    if (x->num < y->num) return -1;
    if (x->num > y->num) return  1;
    if (x->den < y->den) return -1;
    if (x->den > y->den) return  1;
    return 0;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* below offset – defined as 0 */

    /* We must have primes 1..n+1 (0‑based indices 0..n). */
    ensure_primes_by_index(n + 1);
    if (prime_cnt < (size_t)(n + 1)) return (seq_t)0; /* memory failure */

    uint64_t p_n  = prime_cache[n - 1];
    uint64_t p_n1 = prime_cache[n];

    if (n == 1) return (seq_t)1;         /* only one point */

    /* Build all slopes from earlier points to (p_n, p_n1). */
    size_t m = (size_t)(n - 1);                  /* number of earlier points */
    slope_t *slopes = (slope_t*)malloc(m * sizeof(slope_t));
    if (!slopes) return (seq_t)0;                /* memory failure */

    for (size_t i = 0; i < m; ++i) {
        uint64_t p_i  = prime_cache[i];
        uint64_t p_i1 = prime_cache[i + 1];

        int64_t num = (int64_t)p_i1 - (int64_t)p_n1;   /* delta y */
        int64_t den = (int64_t)p_i   - (int64_t)p_n;    /* delta x */

        /* Normalize sign: denominator positive. */
        if (den < 0) { den = -den; num = -num; }

        int64_t g = gcd_int64(num, den);
        if (g != 0) { num /= g; den /= g; }

        slopes[i].num = num;
        slopes[i].den = den;
    }

    /* Find the largest run of equal slopes. */
    qsort(slopes, m, sizeof(slope_t), cmp_slope);
    size_t max_run = 1, cur = 1;
    for (size_t i = 1; i < m; ++i) {
        if (slopes[i].num == slopes[i - 1].num &&
            slopes[i].den == slopes[i - 1].den) {
            ++cur;
        } else {
            if (cur > max_run) max_run = cur;
            cur = 1;
        }
    }
    if (cur > max_run) max_run = cur;

    free(slopes);
    return (seq_t)(max_run + 1);  /* include the target point itself */
}
