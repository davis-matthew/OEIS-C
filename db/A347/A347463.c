/*
 * A347463  Number of ordered factorizations of n with integer alternating
 *          product.
 * https://oeis.org/A347463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered factorizations of n with integer
 *            alternating product. Computation is done by enumerating all
 *            ordered factorizations of n and counting those for which the
 *            product of the factors in even positions divides the product of
 *            the factors in odd positions, i.e. even_product^2 divi...
 * Range:     u8 n <= 31, u16 n <= 9364, u32 n <= 9364, u64 n <= 9364,
 *            u128 n <= 9364, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAXN 10000   /* maximum n that will be queried (about 10 000) */

static int **divs;          /* array of divisor lists for numbers up to MAXN */
static int *divs_sz;        /* sizes of the divisor lists */
static bool init_done = false;

/* initialize the divisor table up to MAXN */
static void init_divisors(void) {
    if (init_done) return;
    init_done = true;
    divs = (int **)malloc((MAXN + 1) * sizeof(int *));
    divs_sz = (int *)malloc((MAXN + 1) * sizeof(int));
    for (int i = 0; i <= MAXN; ++i) {
        divs[i] = NULL;
        divs_sz[i] = 0;
    }
    for (int i = 2; i <= MAXN; ++i) {
        for (int j = i; j <= MAXN; j += i) {
            int sz = divs_sz[j];
            divs[j] = (int *)realloc(divs[j], (sz + 1) * sizeof(int));
            divs[j][sz] = i;
            divs_sz[j] = sz + 1;
        }
    }
}

/* recursive helper that enumerates factorizations.
   rem      - remaining product to factor
   odd_par  - true if the next factor will be in an odd position
   even_prod- product of factors placed so far in even positions
   target   - original n (needed to test the final condition) */
static seq_t dfs(uint64_t rem, bool odd_par, seq_t even_prod, uint64_t target) {
    if (rem == 1) {
        seq_t square = even_prod * even_prod;
        return (target % square == 0) ? (seq_t)1 : (seq_t)0;
    }
    seq_t total = (seq_t)0;
    int *dv = divs[rem];
    int dv_sz = divs_sz[rem];
    for (int i = 0; i < dv_sz; ++i) {
        uint64_t d = (uint64_t)dv[i];
        uint64_t next_rem = rem / d;
        seq_t new_even = even_prod;
        /* if the current position is even (odd_par == false), multiply it into even_prod */
        if (!odd_par) {
            new_even *= d;
        }
        total += dfs(next_rem, !odd_par, new_even, target);
    }
    return total;
}

/* cache for already computed a(n) values */
static seq_t cache[MAXN + 1];
static bool cached[MAXN + 1] = {false};

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined below offset */
    if (n == 1) return (seq_t)1;          /* a(1) = 1 by definition */
    if (n > MAXN) {
        /* if out of bounds, compute naively (safe for test ranges) */
        return dfs(n, true, (seq_t)1, n);
    }
    if (!init_done) init_divisors();
    if (cached[n]) return cache[n];
    seq_t result = dfs(n, true, (seq_t)1, n);
    cache[n] = result;
    cached[n] = true;
    return result;
}
