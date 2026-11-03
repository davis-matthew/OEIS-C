/*
 * A299764  Number of special products of factorizations of n into factors >
 *          1.
 * https://oeis.org/A299764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 77 terms through n = 77 (2026-11-01)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute A299764: Number of special products of factorizations of n into factors > 1
 *  The function enumerates all multiplicative factorizations of n (up to order)
 *  and, for each factorization, counts the subset products that occur exactly once.
 *  It uses a divisor‑based DP to determine the multiplicities of all products.
 *  The implementation is O(number of factorizations × (number of divisors)²), which
 *  is easily fast enough for n ≤ 10000.  The code is C11/C++17 compatible
 *  and respects the required types seq_t and idx_t.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isqrt.h"

/* Return the index of val in the sorted array divs[0..cnt-1].
 * The value is always a divisor of the original number. */
static size_t div_index(const uint64_t *divs, size_t cnt, uint64_t val)
{
    for (size_t i = 0; i < cnt; ++i)
        if (divs[i] == val)
            return i;
    return cnt;   /* should not happen */
}

/* For a given factorization (array factors[0..len-1]), count
 * how many subset products occur exactly once.  The divisor list
 * of the original number n is supplied as divs[0..divcnt-1]. */
static seq_t compute_special_from_factors(const uint64_t *factors, size_t len,
                                          const uint64_t *divs, size_t divcnt)
{
    /* Group identical factor values */
    uint64_t group_vals[64];
    size_t   group_cnts[64];
    size_t   gsize = 0;
    for (size_t i = 0; i < len; ++i) {
        uint64_t v = factors[i];
        size_t j;
        for (j = 0; j < gsize; ++j)
            if (group_vals[j] == v) {
                ++group_cnts[j];
                break;
            }
        if (j == gsize) {
            group_vals[gsize] = v;
            group_cnts[gsize] = 1;
            ++gsize;
        }
    }

    /* Allocate dynamic arrays for DP counts */
    uint32_t *cnt  = (uint32_t *)malloc(divcnt * sizeof(uint32_t));
    uint32_t *newc = (uint32_t *)malloc(divcnt * sizeof(uint32_t));
    if (!cnt || !newc) {
        free(cnt); free(newc);
        return 0;
    }

    for (size_t i = 0; i < divcnt; ++i) cnt[i] = 0;
    cnt[0] = 1;   /* product 1 comes from empty submultiset */

    /* Process each group of equal factors */
    for (size_t gi = 0; gi < gsize; ++gi) {
        uint64_t v = group_vals[gi];
        size_t   e = group_cnts[gi];

        /* Pre‑compute powers v^k for k = 0 .. e */
        uint64_t pow_v[64];
        pow_v[0] = 1;
        for (size_t k = 1; k <= e; ++k)
            pow_v[k] = pow_v[k - 1] * v;

        for (size_t i = 0; i < divcnt; ++i) newc[i] = 0;

        /* Convolve current counts with this group */
        for (size_t di = 0; di < divcnt; ++di) {
            uint32_t c = cnt[di];
            if (!c) continue;
            uint64_t baseProd = divs[di];
            for (size_t k = 0; k <= e; ++k) {
                uint64_t prod = baseProd * pow_v[k];
                size_t idx = div_index(divs, divcnt, prod);
                if (idx != divcnt)
                    newc[idx] += c;
            }
        }

        /* Swap arrays */
        uint32_t *tmp = cnt;
        cnt = newc;
        newc = tmp;
    }

    /* Count products that occur exactly once */
    seq_t total = 0;
    for (size_t i = 0; i < divcnt; ++i)
        if (cnt[i] == 1)
            ++total;

    free(cnt);
    free(newc);
    return total;
}

/* Compute all divisors of n (including 1 and n) in ascending order.
 * The number of divisors is stored in *cnt.
 * The caller must provide an array large enough to hold all divisors.  */
static void divisors_u64(idx_t n, uint64_t *divs, size_t *cnt)
{
    size_t sz = 0;
    for (idx_t i = 1; (i * i) <= n; ++i) {
        if (n % i == 0) {
            divs[sz++] = (uint64_t)i;
            if (i != n / i)
                divs[sz++] = (uint64_t)(n / i);
        }
    }
    /* simple insertion sort – divisor count is tiny */
    for (size_t a = 0; a < sz; ++a)
        for (size_t b = a + 1; b < sz; ++b)
            if (divs[a] > divs[b]) {
                uint64_t tmp = divs[a];
                divs[a] = divs[b];
                divs[b] = tmp;
            }
    *cnt = sz;
}

/* Recursive enumeration of factorizations of n.  The result is accumulated
 * into *acc.  "min_factor" enforces non‑decreasing order to avoid duplicates. */
static void rec_fact(idx_t n, idx_t min_factor, uint64_t *cur, size_t len,
                     const uint64_t *divs, size_t divcnt, seq_t *acc)
{
    /* Add the trivial factorization containing n itself */
    cur[len] = (uint64_t)n;
    *acc += compute_special_from_factors(cur, len + 1, divs, divcnt);

    /* Enumerate further factorizations by trying a prefix factor f */
    idx_t limit = isqrt(n);
    for (idx_t f = min_factor; f <= limit; ++f) {
        if (n % f == 0) {
            cur[len] = (uint64_t)f;
            rec_fact(n / f, f, cur, len + 1, divs, divcnt, acc);
        }
    }
}

/* Public entry point for the sequence */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n < 1)  return (seq_t)0;

    /* Compute all divisors of n */
    uint64_t divs[256];
    size_t   divcnt;
    divisors_u64(n, divs, &divcnt);

    /* Buffer to hold the current factorization (max length < 64 for n ≤ 10000) */
    uint64_t cur[64];

    seq_t total = 0;
    rec_fact(n, 2, cur, 0, divs, divcnt, &total);
    return total;
}
