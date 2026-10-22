/*
 * A346348  Numbers that are the sum of ten fifth powers in exactly three
 *          ways.
 * https://oeis.org/A346348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th smallest number that can be written as the sum
 *            of ten * fifth powers in exactly three (unordered) ways. The
 *            function computes all * such sums by enumerating all multisets
 *            of size ten from {1,…,17} (the largest * base needed for the
 *            first 10 000 terms), counts representations usin...
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   comb_nk
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/comb_nk.h"

/* table entry helper */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* recursive generation of all multisets of size 10 from bases 1..17 */
static void dfs_generate(uint64_t idx, int rem, uint64_t sum,
                         const uint64_t *pow5,
                         size_t table_size,
                         uint64_t *keys, uint8_t *cnts)
{
    if (idx == 17) {                         /* last base: all remaining copies go here */
        sum += (uint64_t)rem * pow5[17];
        size_t h = sum & (table_size - 1);
        while (keys[h] && keys[h] != sum)
            h = (h + 1) & (table_size - 1);
        if (!keys[h]) {
            keys[h] = sum;
            cnts[h] = 1;
        } else if (cnts[h] < 4)              /* cap at 4, we only care about 3 */
            ++cnts[h];
        return;
    }
    for (int c = 0; c <= rem; ++c) {
        dfs_generate(idx + 1, rem - c, sum + (uint64_t)c * pow5[idx],
                     pow5, table_size, keys, cnts);
    }
}

seq_t a(idx_t n)
{
    /* static cache */
    static bool initialized = false;
    static seq_t *terms = NULL;
    static size_t terms_len = 0;

    if (!initialized) {
        const int K = 17;              /* maximal base needed for first 10 000 terms */
        const int NUM_PARTS = 10;

        /* precompute fifth powers */
        uint64_t pow5[18];              /* indices 1..17 used */
        for (int i = 1; i <= K; ++i) {
            uint64_t p = 1;
            for (int j = 0; j < 5; ++j) p *= (uint64_t)i;
            pow5[i] = p;
        }

        /* size of hash table: power of two, > 2 * number of combinations */
        uint64_t sol_cnt = comb_nk((uint64_t)K + 9, NUM_PARTS);
        size_t table_size = 1;
        while (table_size <= sol_cnt * 2) table_size <<= 1;

        uint64_t *keys = (uint64_t *)calloc(table_size, sizeof(uint64_t));
        uint8_t *cnts = (uint8_t *)calloc(table_size, sizeof(uint8_t));
        if (!keys || !cnts) {
            free(keys); free(cnts);
            return 0;
        }

        /* enumerate all multisets and update hash counts */
        dfs_generate(1, NUM_PARTS, 0, pow5,
                     table_size, keys, cnts);

        /* count how many sums occur exactly 3 times */
        size_t count3 = 0;
        for (size_t i = 0; i < table_size; ++i)
            if (keys[i] && cnts[i] == 3)
                ++count3;

        terms = (seq_t *)malloc(count3 * sizeof(seq_t));
        if (!terms) {
            free(keys); free(cnts);
            return 0;
        }

        /* collect sums with count == 3 */
        size_t pos = 0;
        for (size_t i = 0; i < table_size; ++i)
            if (keys[i] && cnts[i] == 3)
                terms[pos++] = (seq_t)keys[i];

        /* sort the terms */
        qsort(terms, pos, sizeof(seq_t), cmp_uint64);
        terms_len = pos;

        free(keys);
        free(cnts);

        initialized = true;
    }

    if (n == 0) return 0;
    if ((idx_t)(n - 1) < (idx_t)terms_len)
        return terms[n - 1];
    return 0;
}
