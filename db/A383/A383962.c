/*
 * A383962  Irregular triangle read by rows: T(n,k) is the index of the k-th
 *          odd divisor in the list of divisors of n, with n >= 1, k >= 1.
 * https://oeis.org/A383962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 108, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the A383962 sequence by expanding the irregular triangle.
 * a(n) = index (1‑based) of the (k‑th) odd divisor of the row r in
 * the list of all divisors of r, where r is the smallest integer such that
 * the cumulative number of odd divisors up to r ≥ n.
 * The calculation uses prime factorisation and explicit divisor enumeration.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/factor_u64.h"

static idx_t count_odd_divisors(idx_t n)
{
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    idx_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if (pr[i] & 1ULL)          /* odd prime factor */
            res *= (idx_t)(ex[i] + 1);
    }
    return res;
}

static void gen_divisors(const uint64_t *pr, const uint64_t *ex, size_t idx,
                         size_t nf, uint64_t curr, uint64_t *divs, size_t *pos)
{
    if (idx == nf) {
        divs[*pos] = curr;
        ++*pos;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t val = curr;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        gen_divisors(pr, ex, idx + 1, nf, val, divs, pos);
        val *= p;
    }
}

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined, but keep sentinel */

    /* static cache of cumulative odd divisor counts per row */
    static idx_t *cum = NULL;
    static size_t cap = 0;
    static size_t len = 0;   /* cum[0] = 0; len = number of entries (cum[0]…cum[len-1]) */

    if (!cum) {
        cap = 128;
        cum = (idx_t *)malloc(cap * sizeof(idx_t));
        cum[0] = 0;
        len = 1;
    }

    /* Extend cache until cumulative sum covers n */
    while (cum[len - 1] < n) {
        idx_t row = len;                     /* next row number */
        idx_t odd_cnt = count_odd_divisors(row);

        if (len >= cap) {
            cap <<= 1;
            cum = (idx_t *)realloc(cum, cap * sizeof(idx_t));
        }
        cum[len] = cum[len - 1] + odd_cnt;
        ++len;
    }

    /* Determine the row r and position k within that row */
    idx_t row = len - 1;                     /* cumulative sum up to this row */
    idx_t prev_sum = cum[row - 1];
    idx_t k = n - prev_sum - 1;              /* 0‑based index within the row */

    /* Factor the row to generate all divisors */
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)row, pr, ex, &cnt);

    /* Compute number of divisors (tau) */
    size_t tau = 1;
    for (size_t i = 0; i < cnt; ++i)
        tau *= (size_t)(ex[i] + 1);

    uint64_t *divs = (uint64_t *)malloc(tau * sizeof(uint64_t));
    size_t pos = 0;
    gen_divisors(pr, ex, 0, cnt, 1ULL, divs, &pos);

    /* Sort all divisors */
    qsort(divs, tau, sizeof(uint64_t), cmp_uint64);

    /* Extract odd divisors (still sorted) */
    uint64_t *odd = (uint64_t *)malloc(tau * sizeof(uint64_t));
    size_t odd_len = 0;
    for (size_t i = 0; i < tau; ++i) {
        if (divs[i] & 1ULL)
            odd[odd_len++] = divs[i];
    }

    /* k-th odd divisor */
    uint64_t target = odd[k];

    /* Find its index among all divisors */
    size_t idx_all = 0;
    for (; idx_all < tau; ++idx_all) {
        if (divs[idx_all] == target)
            break;
    }

    /* Clean up */
    free(divs);
    free(odd);

    return (seq_t)(idx_all + 1);   /* 1‑based index */
}
