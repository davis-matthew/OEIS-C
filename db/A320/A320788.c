/*
 * A320788  Number of multisets of exactly three partitions of positive
 *          integers into distinct parts with total sum of parts equal to n.
 * https://oeis.org/A320788
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of multisets of exactly three partitions of
 *            positive integers into distinct parts with total sum of parts
 *            equal to n. a(n) = [x^n y^3] Product_{j>=1}
 *            1/(1-y*x^j)^{A000009(j)}
 * Range:     u8 n <= 14, u16 n <= 31, u32 n <= 87, u64 n <= 278,
 *            u128 n <= 968, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* dynamic table of partition counts: f[s] = number of partitions of s into distinct parts */
static seq_t *f = NULL;         /* array of size f_cap */
static size_t f_cap = 0;        /* current allocated capacity (size of f) */

/* compute f[s] for 0 <= s <= n */
static void precompute_partitions(idx_t n)
{
    if (f_cap <= (size_t)n) {
        /* reallocate to fit indices up to n */
        seq_t *tmp = (seq_t *)realloc(f, (n + 1) * sizeof(seq_t));
        if (!tmp) return;      /* allocation failed; caller will get a zero */
        f = tmp;
        /* initialize new entries to zero */
        memset(f + f_cap, 0, (n + 1 - f_cap) * sizeof(seq_t));
        f_cap = n + 1;
    }

    /* reset all entries to zero except f[0] */
    for (size_t i = 1; i < f_cap; ++i) f[i] = (seq_t)0;
    f[0] = (seq_t)1;

    /* classic 0‑1 knapsack DP: count partitions into distinct parts */
    for (idx_t part = 1; part <= n; ++part) {
        for (idx_t sum = n; sum >= part; --sum) {
            f[(size_t)sum] += f[(size_t)(sum - part)];
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;

    precompute_partitions(n);

    seq_t total = (seq_t)0;

    for (idx_t i = 1; i <= n - 2; ++i) {
        seq_t di = f[(size_t)i];
        if (di == (seq_t)0) continue;

        for (idx_t j = i; j <= n - i - 1; ++j) {
            seq_t dj = f[(size_t)j];
            if (dj == (seq_t)0) continue;

            idx_t k = n - i - j;
            if (k < j) continue;          /* enforce i <= j <= k */

            seq_t dk = f[(size_t)k];
            if (dk == (seq_t)0) continue;

            if (i == j && j == k) {       /* all equal */
                /* number of unordered triples from di types: C(di+2,3) */
                seq_t tmp = di * (di + (seq_t)1);
                tmp = tmp * (di + (seq_t)2);
                tmp = tmp / (seq_t)6;
                total += tmp;
            }
            else if (i == j) {            /* i == j < k */
                /* two from di (with repetitions) and one from dk */
                seq_t pair = di * (di + (seq_t)1);
                pair = pair / (seq_t)2;
                total += pair * dk;
            }
            else if (j == k) {            /* i < j == k */
                seq_t pair = dj * (dj + (seq_t)1);
                pair = pair / (seq_t)2;
                total += pair * di;
            }
            else {                         /* all distinct */
                total += di * dj * dk;
            }
        }
    }

    return total;
}
