/*
 * A379671  Array read by antidiagonals downward where A(n,k) is the number of
 *          finite sets of positive integers with sum n and product k.
 * https://oeis.org/A379671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*********************************************************************
 *  Formula: a(n) is the entry obtained by reading the infinite
 *  array A(n,k) (number of finite sets of positive integers with
 *  sum n and product k) along antidiagonals downward, starting
 *  with A(0,1)=1.  The sequence is 1-indexed.
 *********************************************************************/

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  Factorisation helper – returns prime factors of n                */
static void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    *cnt = 0;
    if (n < 2) return;

    /* factor 2 */
    if ((n & 1ULL) == 0) {
        pr[*cnt] = 2ULL;
        ex[*cnt] = 0;
        while ((n & 1ULL) == 0) {
            ++ex[*cnt];
            n >>= 1;
        }
        ++(*cnt);
    }

    /* factor odd primes */
    for (uint64_t p = 3; (__uint128_t)p * p <= n; p += 2) {
        if (n % p == 0) {
            pr[*cnt] = p;
            ex[*cnt] = 0;
            while (n % p == 0) {
                ++ex[*cnt];
                n /= p;
            }
            ++(*cnt);
        }
    }

    /* remaining prime > sqrt(n) */
    if (n > 1) {
        pr[*cnt] = n;
        ex[*cnt] = 1;
        ++(*cnt);
    }
}

/* ------------------------------------------------------------------ */
/*  Return (row,column) on the antidiagonal reading, 0‑based           */
static void antidiagonal_coords(idx_t idx, idx_t *row, idx_t *col)
{
    idx_t seen = 0;
    for (idx_t d = 0; ; ++d) {
        idx_t len = d + 1;
        if (seen + len > idx) {
            *row = idx - seen;        /* 0‑based row (sum)     */
            *col = d - *row;          /* 0‑based column (prod-1) */
            return;
        }
        seen += len;
    }
}

/* ------------------------------------------------------------------ */
/*  Comparison for qsort on uint64_t                                 */
static int cmp_uint64(const void *a, const void *b)
{
    const uint64_t *va = (const uint64_t *)a;
    const uint64_t *vb = (const uint64_t *)b;
    return (*va > *vb) - (*va < *vb);
}

/* ------------------------------------------------------------------ */
/*  Recursive enumeration of subsets of divisors of k                */
static uint64_t dfs_rec(const uint64_t *divs, size_t dsize,
                        size_t pos, uint64_t rem, int sum,
                        int target_sum)
{
    if (pos == dsize) {
        return (rem == 1 && sum == target_sum) ? 1ULL : 0ULL;
    }

    /* skip current divisor */
    uint64_t res = dfs_rec(divs, dsize, pos + 1, rem, sum, target_sum);

    /* include current divisor */
    uint64_t d = divs[pos];
    if (rem % d == 0) {
        uint64_t new_rem = rem / d;
        int new_sum = sum + (int)d;
        if (new_sum <= target_sum)
            res += dfs_rec(divs, dsize, pos + 1, new_rem, new_sum, target_sum);
    }
    return res;
}

/* ------------------------------------------------------------------ */
/*  Count the sets with given sum and product                         */
static uint64_t count_sets_int(int sum_target, uint64_t prod_target)
{
    /* 1. factorise product */
    uint64_t primes[64], exps[64];
    size_t pf_cnt;
    factor_u64(prod_target, primes, exps, &pf_cnt);

    /* 2. generate all divisors of prod_target */
    size_t div_capacity = 1;
    for (size_t i = 0; i < pf_cnt; ++i)
        div_capacity *= (exps[i] + 1);

    uint64_t *divs = (uint64_t *)malloc(div_capacity * sizeof(uint64_t));
    size_t dsize = 0;
    divs[dsize++] = 1ULL;
    for (size_t i = 0; i < pf_cnt; ++i) {
        size_t old = dsize;
        uint64_t pow = 1ULL;
        for (uint64_t e = 1; e <= exps[i]; ++e) {
            pow *= primes[i];
            for (size_t j = 0; j < old; ++j)
                divs[dsize++] = divs[j] * pow;
        }
    }

    /* 3. sort divisors to have a deterministic order */
    qsort(divs, dsize, sizeof(uint64_t), cmp_uint64);

    /* 4. count subsets whose product is prod_target and sum is sum_target */
    uint64_t result = dfs_rec(divs, dsize, 0, prod_target, 0, sum_target);

    free(divs);
    return result;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                              */
seq_t a(idx_t n)
{
    /* 1‑based index n ≥ 1 ; zero‑based counter idx = n-1          */
    idx_t idx = n - 1;
    idx_t row, col;
    antidiagonal_coords(idx, &row, &col);

    int  sum  = (int)row;            /* sum of the set      */
    uint64_t prod = (uint64_t)col + 1ULL;  /* product of the set */

    uint64_t cnt = count_sets_int(sum, prod);
    return (seq_t)cnt;
}
