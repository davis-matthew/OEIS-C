/*
 * A319137  Number of strict planar branching factorizations of n.
 * https://oeis.org/A319137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + sum over all unordered sets {d1,…,dk} (k ≥ 2) of
 *            distinct factors d_i ≥ 2 with product n, of (k! × ∏ a(d_i))
 *            (recurrence)
 * Range:     u8 n <= 23, u16 n <= 11, u32 n <= 5, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* maximum number of distinct factors needed for n ≤ 10000 (safe upper bound) */
#define MAX_K 15

static seq_t *a_cache = NULL;        /* cache of a(0 … m) */
static idx_t a_cache_size = 0;       /* current size of a_cache */

/* pre‑computed factorials 0! … MAX_K! */
static seq_t fact[MAX_K + 1] = {0};
static bool fact_init = false;

/* recursive enumeration of unordered distinct factor sets of product `rem` */
static void dfs_factors(const idx_t *divs, size_t div_cnt,
                        size_t start, idx_t rem, size_t sz,
                        seq_t prod_a, seq_t *sum)
{
    if (rem == 1) {
        if (sz >= 2) {
            *sum += prod_a * fact[sz];
        }
        return;
    }

    for (size_t i = start; i < div_cnt; ++i) {
        idx_t d = divs[i];
        if (d > rem) break;
        if (rem % d != 0) continue;
        /* use product of g(d) = a(d) and continue with reduced product */
        dfs_factors(divs, div_cnt, i + 1,
                    rem / d, sz + 1,
                    prod_a * a_cache[d], sum);
    }
}

/* main function computing a(n) */
seq_t a(idx_t n)
{
    /* initialise factorial table once */
    if (!fact_init) {
        fact[0] = 1;
        for (size_t i = 1; i <= MAX_K; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_init = true;
    }

    /* expand cache if needed */
    if (n + 1 > a_cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = NULL;

        new_cache = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0;   /* allocation failed: return 0 */

        /* compute values for new indices */
        for (idx_t i = a_cache_size; i < new_size; ++i) {
            if (i == 0) {
                new_cache[i] = 0;  /* unused */
                continue;
            }
            if (i == 1) {
                new_cache[i] = 1;
                continue;
            }

            /* collect all divisors d ≥ 2 of i (including i itself) */
            idx_t divisors[128];
            size_t dcnt = 0;
            for (idx_t d = 2; d * d <= i; ++d) {
                if (i % d == 0) {
                    divisors[dcnt++] = d;
                    if (d != i / d) divisors[dcnt++] = i / d;
                }
            }
            divisors[dcnt++] = i;   /* include the number itself */

            /* simple insertion sort for the small divisor array */
            for (size_t p = 0; p < dcnt; ++p) {
                for (size_t q = p + 1; q < dcnt; ++q) {
                    if (divisors[p] > divisors[q]) {
                        idx_t tmp = divisors[p];
                        divisors[p] = divisors[q];
                        divisors[q] = tmp;
                    }
                }
            }

            seq_t sum = 0;
            dfs_factors(divisors, dcnt, 0, i, 0, 1, &sum);

            new_cache[i] = sum + 1;
        }

        a_cache = new_cache;
        a_cache_size = new_size;
    }

    return a_cache[n];
}
