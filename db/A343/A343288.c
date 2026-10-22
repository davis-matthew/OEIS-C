/*
 * A343288  Dirichlet g.f.: Product_{k>=2} 1 / (1 - k^(-s))^(k^8).
 * https://oeis.org/A343288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binom_k8_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A343288 (Dirichlet generating function product_{k>=2}
   1/(1 - k^(-s))^(k^8)).  
   The coefficient a(n) is the sum over all multiplicative partitions
   n = ∏ k^{e_k} of ∏ binom(k^8 + e_k - 1, e_k). */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binom_k8_exp.h"

/* compare integers for qsort */
static int cmp_int(const void *a, const void *b)
{
    return (*(int *)a) - (*(int *)b);
}

/* recursive computation of a(n) with lower bound k_min */
static seq_t dfs(idx_t n, idx_t k_min)
{
    if (n == 1)
        return (seq_t)1;
    if (k_min > n)
        return (seq_t)0;

    int divs[128];
    int dcnt = 0;
    /* collect divisors >1 of n */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            int d1 = (int)d;
            int d2 = (int)(n / d);
            if (d1 > 1)
                divs[dcnt++] = d1;
            if (d2 != d1 && d2 > 1)
                divs[dcnt++] = d2;
        }
    }
    qsort(divs, dcnt, sizeof(int), cmp_int);

    seq_t total = 0;
    for (int i = 0; i < dcnt; ++i) {
        int k = divs[i];
        if (k < k_min)
            continue;

        /* maximum exponent of k dividing n */
        idx_t temp = n;
        int emax = 0;
        while (temp % (idx_t)k == 0) {
            temp /= (idx_t)k;
            ++emax;
        }

        idx_t powk = 1;
        for (int e = 1; e <= emax; ++e) {
            powk *= (idx_t)k;
            idx_t rem = n / powk;
            seq_t bin = binom_k8_exp((idx_t)k, e);
            seq_t sub = dfs(rem, (idx_t)k + 1);
            total += bin * sub;
        }
    }
    return total;
}

seq_t a(idx_t n)
{
    return dfs(n, 2);
}
