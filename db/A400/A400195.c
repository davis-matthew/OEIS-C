/*
 * A400195  Maximum number of odd terms in a 2-dense sublist of divisors of n.
 * https://oeis.org/A400195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) is the maximum number of odd divisors in any 2‑dense
 *            sublist of the sorted list of all divisors of n. A sublist is
 *            2‑dense if each consecutive pair d[i+1] ≤ 2·d[i]. The routine
 *            factors n, enumerates all divisors, sorts them, and then scans
 *            each maximal consecutive block that satisfies the de...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9674 (2026-11-01)
 * Helpers:   factor_u64, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/num_divisors.h"

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

/* Comparator for qsort on arrays of idx_t */
static int cmp_idx_t(const void *a, const void *b)
{
    idx_t x = *(const idx_t *)a;
    idx_t y = *(const idx_t *)b;
    return (x > y) - (x < y);
}

/* Recursive helper to generate all divisors of n.
   The divisions are stored in an array of idx_t type to avoid overflow
   even when seq_t is small. */
static void _gen_divs(const uint64_t *pr, const uint64_t *ex,
                      size_t fac_cnt, size_t pos,
                      idx_t cur, idx_t *arr, size_t *cnt)
{
    if (pos == fac_cnt) {
        arr[*cnt] = cur;
        (*cnt)++;
        return;
    }
    idx_t val = cur;
    for (uint64_t e = 0; e <= ex[pos]; ++e) {
        _gen_divs(pr, ex, fac_cnt, pos + 1, val, arr, cnt);
        val = val * (idx_t)pr[pos];
    }
}

seq_t a(idx_t n)
{
    /* 1. Factorise n */
    uint64_t primes[64];
    uint64_t exps   [64];
    size_t fac_cnt;
    factor_u64((uint64_t)n, primes, exps, &fac_cnt);

    /* 2. Compute number of divisors and allocate array */
    size_t dcnt = (size_t)num_divisors((uint64_t)n);
    if (dcnt == 0) return (seq_t)0;            /* should not happen for n≥1 */

    idx_t *divs = (idx_t *)malloc(dcnt * sizeof(idx_t));
    if (!divs) return (seq_t)0;                /* memory failure */

    /* 3. Generate all divisors */
    size_t num = 0;
    _gen_divs(primes, exps, fac_cnt, 0, (idx_t)1, divs, &num);

    /* 4. Sort the divisors */
    qsort(divs, dcnt, sizeof(idx_t), cmp_idx_t);

    /* 5. Scan 2‑dense windows and count odd divisors */
    seq_t maxOdd = 0;
    size_t i = 0;
    while (i < dcnt) {
        size_t j = i;
        seq_t oddCnt = 0;
        while (j < dcnt &&
               (j == i || (idx_t)divs[j] <= (idx_t)2 * (idx_t)divs[j-1])) {
            if ((divs[j] & (idx_t)1) != 0)
                oddCnt++;
            j++;
        }
        if (oddCnt > maxOdd)
            maxOdd = oddCnt;
        i = j;                     /* move to next window start */
    }

    free(divs);
    return maxOdd;
}
