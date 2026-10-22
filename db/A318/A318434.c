/*
 * A318434  Number of ways to split the integer partition with Heinz number n
 *          into consecutive subsequences with equal sums.
 * https://oeis.org/A318434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A318434: Number of ways to split the partition with Heinz number n
   into consecutive subsequences with equal sums.  The function follows the
   definition in the OEIS entry; it does not use any hard‑coded values.   */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* Comparator for qsort to sort indices in descending order. */
static int cmp_desc(const void *a, const void *b)
{
    const idx_t *ia = (const idx_t *)a;
    const idx_t *ib = (const idx_t *)b;
    if (*ia < *ib) return 1;
    if (*ia > *ib) return -1;
    return 0;
}

/* Returns the number of constant‑sum splits of the partition corresponding
   to the Heinz number n.  The partition is built from the multiset of
   prime indices of the prime factorisation of n, sorted in non‑increasing
   order.  */
seq_t a(idx_t n)
{
    /* The empty partition (n = 1) has one trivial split. */
    if (n <= 1u) return (seq_t)1;

    /* ---------- Build list of primes up to n ---------- */
    uint64_t *prime_list = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &prime_list, &prime_cnt);

    /* ---------- Factor n and build the parts array ---------- */
    /* The partition length is at most the total exponent sum.  For n ≤ 10000
       this is safely below 64. */
    idx_t parts[64];
    size_t parts_len = 0;
    idx_t sum_total = 0;

    uint64_t m = n;
    for (size_t pi = 0; pi < prime_cnt &&
                     (uint64_t)prime_list[pi] * (uint64_t)prime_list[pi] <= m;
         ++pi)
    {
        uint64_t p = prime_list[pi];
        if (m % p == 0)
        {
            int exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
            }

            /* Find the index of this prime (1‑based). */
            size_t idx = 0;
            for (size_t k = 0; k < prime_cnt; ++k)
                if (prime_list[k] == p) { idx = k + 1; break; }

            /* Append idx 'exp' times to the parts array. */
            for (int e = 0; e < exp; ++e)
            {
                parts[parts_len++]  = (idx_t)idx;
                sum_total += (idx_t)idx;
            }
        }
    }

    /* Handle a remaining prime factor > sqrt(n). */
    if (m > 1u)
    {
        uint64_t p = m;
        size_t idx = 0;
        for (size_t k = 0; k < prime_cnt; ++k)
            if (prime_list[k] == p) { idx = k + 1; break; }
        parts[parts_len++]  = (idx_t)idx;
        sum_total += (idx_t)idx;
    }

    free(prime_list);

    /* ---------- Sort the partition in descending order ---------- */
    if (parts_len > 1) qsort(parts, parts_len, sizeof(idx_t), cmp_desc);

    /* ---------- Count valid constant‑sum splits ---------- */
    seq_t answer = 0;
    for (size_t t = 1; t <= parts_len; ++t)
    {
        if (sum_total % t != 0) continue;

        idx_t block_sum = sum_total / t;
        idx_t acc        = 0;
        size_t blocks    = 0;
        bool ok = true;

        for (size_t i = 0; i < parts_len; ++i)
        {
            acc += parts[i];
            if (acc == block_sum)
            {
                ++blocks;
                acc = 0;
            }
            else if (acc > block_sum)
            {
                ok = false;
                break;
            }
        }

        if (ok && blocks == t && acc == 0)
            answer += (seq_t)1;
    }

    return answer;
}
