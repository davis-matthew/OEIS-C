/*
 * A371195  List of values q^5-q^4 as q runs through A000961 (that is, 1,
 *          primes, and prime powers).
 * https://oeis.org/A371195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = q(n)^5 - q(n)^4, where q(n) is the n‑th term of OEIS
 *            A000961 (1, the primes and prime powers)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

static int cmp_idx_t(const void *p1, const void *p2)
{
    const idx_t *a = (const idx_t *)p1;
    const idx_t *b = (const idx_t *)p2;
    return (*a < *b) ? -1 : (*a > *b) ? 1 : 0;
}

/* Generate all A000961 values ≤ limit.  The array is sorted ascending. */
static void generate_prime_powers(idx_t limit, idx_t **out, size_t *cnt)
{
    size_t sz = (size_t)(limit + 1);
    bool *is_prime = (bool *)malloc(sz * sizeof(bool));
    if (!is_prime) exit(1);
    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = true;

    for (idx_t p = 2; (uint64_t)p * p <= (uint64_t)limit; ++p) {
        if (is_prime[p]) {
            for (idx_t m = p * p; m <= limit; m += p) is_prime[m] = false;
        }
    }

    size_t pr_cap = limit / 10 + 1;
    if (pr_cap < 256) pr_cap = 256;
    idx_t *primes = (idx_t *)malloc(pr_cap * sizeof(idx_t));
    if (!primes) { free(is_prime); exit(1); }
    size_t pr_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            if (pr_cnt == pr_cap) {
                pr_cap *= 2;
                primes = (idx_t *)realloc(primes, pr_cap * sizeof(idx_t));
                if (!primes) { free(is_prime); exit(1); }
            }
            primes[pr_cnt++] = i;
        }
    }

    size_t arr_cap = pr_cnt + 200;
    idx_t *arr = (idx_t *)malloc(arr_cap * sizeof(idx_t));
    if (!arr) { free(is_prime); free(primes); exit(1); }
    size_t arr_cnt = 0;

    arr[arr_cnt++] = 1;                     /* include 1 */

    for (size_t pi = 0; pi < pr_cnt; ++pi) {
        idx_t p = primes[pi];
        idx_t pow = p;
        while (pow <= limit) {
            if (arr_cnt == arr_cap) {
                arr_cap *= 2;
                arr = (idx_t *)realloc(arr, arr_cap * sizeof(idx_t));
                if (!arr) { free(is_prime); free(primes); exit(1); }
            }
            arr[arr_cnt++] = pow;
            if (pow > limit / p) break;
            pow *= p;
        }
    }

    free(is_prime);
    free(primes);

    qsort(arr, arr_cnt, sizeof(idx_t), cmp_idx_t);

    *out = arr;
    *cnt = arr_cnt;
}

/* Cache of prime powers */
static idx_t *qvals   = NULL;
static size_t  qcnt   = 0;
static idx_t   bound  = 1000;   /* starting bound */

/* Ensure at least n prime powers are cached. */
static void ensure_qvals(idx_t n)
{
    if (n <= qcnt) return;
    while (qcnt < n) {
        idx_t newbound = bound * 2;
        idx_t *tmp;
        size_t tmpcnt;
        generate_prime_powers(newbound, &tmp, &tmpcnt);
        free(qvals);
        qvals = tmp;
        qcnt  = tmpcnt;
        bound = newbound;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset guard */

    ensure_qvals(n);
    idx_t q = qvals[n - 1];          /* a(1) corresponds to q = 1 */

    /* compute q^4 */
    seq_t q4 = (seq_t)q;
    q4 *= q;   /* q^2 */
    q4 *= q;   /* q^3 */
    q4 *= q;   /* q^4 */

    /* a(n) = q^5 - q^4 = q^4 * (q - 1) */
    seq_t result = q4 * (seq_t)(q - 1);
    return result;
}
