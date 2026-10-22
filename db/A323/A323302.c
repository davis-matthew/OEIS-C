/*
 * A323302  Number of ways to arrange the parts of the integer partition with
 *          Heinz number n into a matrix with equal row-sums and equal
 *          column-sums.
 * https://oeis.org/A323302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to arrange the parts of the integer *
 *            partition with Heinz number n into a matrix with equal row‑sums
 *            and * equal column‑sums
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* Utilities used by the project */
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"

/* Global storage for the list of primes up to 10000. */
static uint64_t *PRIME_LIST = NULL;
static size_t PRIME_COUNT = 0;
static bool PRIMES_INITIALIZED = false;

/* Helper to initialise the primes list once. */
static void init_primes(void)
{
    if (!PRIMES_INITIALIZED) {
        build_primes_upto(10000, (uint64_t **)&PRIME_LIST, &PRIME_COUNT);
        PRIMES_INITIALIZED = true;
    }
}

/* Return the 1‑based index of a prime p in the primes list. */
static int prime_index(uint64_t p)
{
    for (size_t i = 0; i < PRIME_COUNT; ++i)
        if (PRIME_LIST[i] == p)
            return (int)(i + 1);   /* 1‑based */
    return -1;  /* should never happen for valid input */
}

/* Recursive search for a single (r,c) shape. */
static void dfs_rec(int pos, int k, int r, int c,
                    const int *values, int *remCnt, int uniqSize,
                    seq_t R, seq_t C,
                    seq_t *rowSums, seq_t *colSums,
                    seq_t *count)
{
    if (pos == k) {                 /* all cells filled */
        (*count)++;                  /* a valid matrix found */
        return;
    }

    int row = pos / c;
    int col = pos % c;

    for (int i = 0; i < uniqSize; ++i) {
        if (remCnt[i] == 0)
            continue;
        int v = values[i];
        seq_t vseq = (seq_t)v;

        if (rowSums[row] + vseq > R)
            continue;
        if (colSums[col] + vseq > C)
            continue;

        /* place the value */
        remCnt[i]--;
        rowSums[row] += vseq;
        colSums[col] += vseq;

        bool ok = true;
        if (col == c - 1 && rowSums[row] != R)
            ok = false;
        if (row == r - 1 && colSums[col] != C)
            ok = false;

        if (ok)
            dfs_rec(pos + 1, k, r, c, values, remCnt, uniqSize,
                    R, C, rowSums, colSums, count);

        /* backtrack */
        remCnt[i]++;
        rowSums[row] -= vseq;
        colSums[col] -= vseq;
    }
}

/* Main entry function for the sequence. */
seq_t a(idx_t n)
{
    if (n == 1)                 /* the empty partition is counted as one matrix */
        return (seq_t)1;

    init_primes();

    /* Factorize n. */
    uint64_t primes[20], exponents[20];
    size_t pf_cnt = 0;
    factor_u64(n, primes, exponents, &pf_cnt);

    /* Build the multiset of parts (prime indices). */
    int parts[20];
    int k = 0;
    for (size_t i = 0; i < pf_cnt; ++i) {
        int idx = prime_index(primes[i]);
        for (uint64_t e = 0; e < exponents[i]; ++e)
            parts[k++] = idx;
    }

    /* Sum of parts. */
    seq_t sumParts = 0;
    for (int i = 0; i < k; ++i)
        sumParts += (seq_t)parts[i];

    /* Build the list of distinct values and their counts. */
    int uniqVals[20], uniqCnts[20];
    int uniqCnt = 0;
    for (int i = 0; i < k; ++i) {
        int v = parts[i];
        bool found = false;
        for (int j = 0; j < uniqCnt; ++j) {
            if (uniqVals[j] == v) {
                uniqCnts[j]++;
                found = true;
                break;
            }
        }
        if (!found) {
            uniqVals[uniqCnt] = v;
            uniqCnts[uniqCnt] = 1;
            uniqCnt++;
        }
    }

    seq_t total = 0;

    /* Test all (r,c) with r*c==k. */
    for (idx_t r = 1; r <= (idx_t)k; ++r) {
        if (k % r != 0) continue;
        idx_t c = (idx_t)k / r;
        if ( (sumParts % (seq_t)r != 0) || (sumParts % (seq_t)c != 0) )
            continue;
        seq_t R = sumParts / (seq_t)r;
        seq_t C = sumParts / (seq_t)c;

        int remCnt[20];
        memcpy(remCnt, uniqCnts, sizeof(uniqCnts));

        seq_t rowSums[20] = {0};
        seq_t colSums[20] = {0};
        seq_t count = 0;

        dfs_rec(0, k, (int)r, (int)c,
                uniqVals, remCnt, uniqCnt,
                R, C,
                rowSums, colSums,
                &count);

        total += count;
    }

    return total;
}
