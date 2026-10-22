/*
 * A342530  Number of strict chains of divisors ending with n and having
 *          distinct first quotients.
 * https://oeis.org/A342530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A254578(d) where A254578(d) = number of strict
 *            chains of divisors ending with d and having distinct first
 *            quotients. This is equivalent to the number of ordered
 *            factorizations of d into distinct factors >1 (the factorization
 *            consisting only of d itself counts as one)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t fact_table[7] = {0};

/* initialise factorial table up to 6! (sufficient for n <= 10000) */
static void init_factorial(void)
{
    if (fact_table[1]) return;   /* already initialised */
    fact_table[0] = (seq_t)1;
    for (int i = 1; i < 7; ++i)
        fact_table[i] = fact_table[i - 1] * (seq_t)i;
}

/* cache for A254578 values */
static seq_t *f_cache = NULL;     /* cache[0] unused, indices 1..n */
static size_t f_cache_cap = 0;    /* capacity of cache (size of array) */

/* ensure cache can hold indices up to n */
static void ensure_f_cache(idx_t n)
{
    if (f_cache_cap > n) return;
    size_t new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(f_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                /* allocation failure */
    for (size_t i = f_cache_cap; i < new_cap; ++i)
        tmp[i] = 0;
    f_cache = tmp;
    f_cache_cap = new_cap;
}

/* recursive helper to count ordered factorizations of target using
   distinct divisors stored in divs[0..dcnt-1] (strictly increasing). */
static void dfs_helper(idx_t target, const int *divs, int dcnt,
                       int start, idx_t prod, int len, seq_t *res)
{
    for (int i = start; i < dcnt; ++i) {
        int d = divs[i];
        if (prod > target / (idx_t)d)
            continue;                /* would exceed target */
        idx_t newp = prod * (idx_t)d;
        if (newp == target) {
            *res += fact_table[len + 1];  /* len+1 distinct factors */
        } else {
            dfs_helper(target, divs, dcnt, i + 1, newp, len + 1, res);
        }
    }
}

/* compute A254578(n) and store in cache */
static seq_t compute_f_canonical(idx_t n)
{
    if (n == 1) return (seq_t)1;        /* f(1) = 1 */

    /* collect divisors > 1 of n */
    int divs[128];
    int dcnt = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            if (d1 > 1) {
                if (dcnt < 128) divs[dcnt++] = (int)d1;
            }
            idx_t d2 = n / i;
            if (d2 != d1 && d2 > 1) {
                if (dcnt < 128) divs[dcnt++] = (int)d2;
            }
        }
    }

    /* sort divisors ascending (bubble sort, size <=128) */
    for (int a = 0; a < dcnt; ++a)
        for (int b = a + 1; b < dcnt; ++b)
            if (divs[a] > divs[b]) {
                int t = divs[a];
                divs[a] = divs[b];
                divs[b] = t;
            }

    seq_t res = 0;
    dfs_helper(n, divs, dcnt, 0, (idx_t)1, 0, &res);
    return res;
}

/* public sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;  /* undefined for n < 1 */

    if (!fact_table[1]) init_factorial();

    ensure_f_cache(n);

    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            if (f_cache[d1] == 0)
                f_cache[d1] = compute_f_canonical(d1);
            sum += f_cache[d1];

            idx_t d2 = n / i;
            if (d2 != d1) {
                if (f_cache[d2] == 0)
                    f_cache[d2] = compute_f_canonical(d2);
                sum += f_cache[d2];
            }
        }
    }
    return sum;
}
