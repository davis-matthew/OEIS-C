/*
 * A390494  Irregular triangle read by rows: T(n,k) is the sum of nonprime
 *          numbers in the k-th 2-dense sublist of divisors of n, with n >= 1,
 *          k >= 1.
 * https://oeis.org/A390494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = the n-th entry of the irregular triangle read by rows
 *            (OEIS A390494). The triangle has rows indexed by the integer n;
 *            row n has as many entries as there are maximal 2‑dense sublists
 *            of the sorted list of divisors of n. Each entry T(n,k) is the
 *            sum of nonprime divisors in the k‑th sublist....
 * Range:     u8 n <= 234, u16 n <= 9933, u32 n <= 9933, u64 n <= 9933,
 *            u128 n <= 9933, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static const size_t MAX_FACTORS = 16;

static void factor_uint64(uint64_t n, uint64_t *primes, int *exps, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[c] = p;
            exps[c] = e;
            ++c;
            if (c >= MAX_FACTORS) break;   /* safe guard */
        }
    }
    if (n > 1 && c < MAX_FACTORS) {
        primes[c] = n;
        exps[c] = 1;
        ++c;
    }
    *cnt = c;
}

static int cmp_uint64(const void *a, const void *b)
{
    const uint64_t *x = (const uint64_t *)a;
    const uint64_t *y = (const uint64_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

static void gen_divisors_rec(const uint64_t *primes, const int *exps,
                             size_t cnt, uint64_t cur, size_t idx,
                             uint64_t **divs, size_t *dc, size_t *cap)
{
    if (idx == cnt) {
        if (*dc >= *cap) {
            *cap = (*cap == 0) ? 8 : (*cap << 1);
            *divs = (uint64_t *)realloc(*divs, *cap * sizeof(uint64_t));
        }
        (*divs)[(*dc)++] = cur;
        return;
    }
    uint64_t p = primes[idx];
    uint64_t val = cur;
    for (int e = 0; e <= exps[idx]; ++e) {
        gen_divisors_rec(primes, exps, cnt, val, idx + 1, divs, dc, cap);
        val *= p;
    }
}

static void get_divisors(uint64_t n, uint64_t **divisors, size_t *len)
{
    uint64_t primes[MAX_FACTORS];
    int exps[MAX_FACTORS];
    size_t cnt = 0;
    factor_uint64(n, primes, exps, &cnt);

    uint64_t *divs = NULL;
    size_t dcnt = 0, dcap = 0;
    gen_divisors_rec(primes, exps, cnt, 1ULL, 0, &divs, &dcnt, &dcap);

    if (dcnt > 1)
        qsort(divs, dcnt, sizeof(uint64_t), cmp_uint64);

    *divisors = divs;
    *len = dcnt;
}

static idx_t row_length(uint64_t n)
{
    uint64_t *divisors;
    size_t len;
    get_divisors(n, &divisors, &len);

    idx_t segcnt = 0;
    for (size_t i = 0; i < len; ++i) {
        if (i == 0 || divisors[i] > divisors[i - 1] * 2ULL) {
            ++segcnt;
        }
    }

    free(divisors);
    return segcnt;
}

static seq_t row_k_sum(uint64_t n, idx_t k)   /* k is zero‑based */
{
    uint64_t *divs;
    size_t len;
    get_divisors(n, &divs, &len);

    idx_t segidx = 0;
    seq_t segsum = 0;
    for (size_t i = 0; i < len; ++i) {
        uint64_t d = divs[i];
        if (!is_prime(d))
            segsum += (seq_t)d;

        bool flush = false;
        if (i + 1 == len)           /* last element */
            flush = true;
        else {
            uint64_t next = divs[i + 1];
            if (next > (uint64_t)(((unsigned __int128)d) * 2ULL))
                flush = true;
        }

        if (flush) {
            if (segidx == k) {
                free(divs);
                return segsum;
            }
            ++segidx;
            segsum = 0;
        }
    }
    free(divs);
    return 0;   /* should not reach here for valid k */
}

static idx_t *cum_lengths = NULL;      /* cumulative lengths for rows */
static size_t cum_cap = 0;
static size_t cum_len = 0;              /* number of entries stored (rows processed) */

/* Ensure cum_lengths[0..row] are filled up to at least target_len */
static void ensure_rows(idx_t target_len)
{
    if (cum_len == 0) {
        /* init with cum_lengths[0] = 0 */
        cum_cap = 8;
        cum_lengths = (idx_t *)calloc(cum_cap, sizeof(idx_t));
        cum_lengths[0] = 0;
        cum_len = 1;
    }

    while (cum_lengths[cum_len - 1] < target_len) {
        idx_t row = (idx_t)(cum_len);     /* next row number (1‑based) */
        idx_t len_row = row_length((uint64_t)row);

        if (cum_len == cum_cap) {
            cum_cap <<= 1;
            cum_lengths = (idx_t *)realloc(cum_lengths, cum_cap * sizeof(idx_t));
        }
        cum_lengths[cum_len] = cum_lengths[cum_len - 1] + len_row;
        ++cum_len;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, but guard for safety */

    ensure_rows(n);

    /* Find row r where cum_lengths[r-1] < n <= cum_lengths[r] */
    idx_t r = 1;
    while (cum_lengths[r] < n) ++r;

    idx_t prev = cum_lengths[r - 1];
    idx_t k = n - prev - 1;   /* zero‑based index within row r */

    return row_k_sum((uint64_t)r, k);
}
