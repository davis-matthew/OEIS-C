/*
 * A347454  Numbers whose multiset of prime indices has integer alternating
 *          product.
 * https://oeis.org/A347454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the n-th integer whose multiset of prime indices has integer alternating product.
   The algorithm searches integers in order and tests the property by factoring each integer.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Global data structures */
static size_t prime_cnt = 0;
static uint32_t *primes = NULL;      /* list of primes up to LIMIT */
static int *prime_index_of = NULL;   /* prime → index (1‑based) */
static int *counts = NULL;           /* counts of each prime index in current n */
static int *usedIdx = NULL;          /* indices of indices actually used */
static size_t usedIdx_len = 0;
static int *diff = NULL;             /* exponent difference for each prime value */
static int *usedDiff = NULL;         /* primes whose diff changed */
static size_t usedDiff_len = 0;
static bool globals_initialized = false;

/* Sieve of Eratosthenes up to LIMIT */
static void init_globals(void)
{
    const int LIMIT = 200000;          /* sufficient for the required search range */
    int *is_comp = (int *)calloc(LIMIT + 1, sizeof(int));
    if (!is_comp) exit(1);
    for (int i = 2; i * i <= LIMIT; ++i)
        if (!is_comp[i])
            for (int j = i * i; j <= LIMIT; j += i)
                is_comp[j] = 1;

    /* count primes */
    size_t ptmp = 0;
    for (int i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) ++ptmp;
    prime_cnt = ptmp;

    primes = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    prime_index_of = (int *)calloc(LIMIT + 1, sizeof(int));

    uint32_t idx = 1;
    size_t pi = 0;
    for (int i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            primes[pi++] = (uint32_t)i;
            prime_index_of[i] = (int)idx++;
        }
    }
    free(is_comp);

    /* auxiliary arrays */
    counts = (int *)calloc(prime_cnt + 1, sizeof(int));
    usedIdx = (int *)malloc((prime_cnt + 1) * sizeof(int));
    diff = (int *)calloc(prime_cnt + 1, sizeof(int));
    usedDiff = (int *)malloc((prime_cnt + 1) * sizeof(int));
}

/* Check whether the integer m satisfies the property */
static bool check_property(uint64_t m)
{
    if (m == 1) return true;

    uint64_t tmp = m;
    /* factor m */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint32_t p = primes[pi];
        if ((uint64_t)p * (uint64_t)p > tmp) break;
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            int idx = prime_index_of[p];
            if (counts[idx] == 0) usedIdx[usedIdx_len++] = idx;
            counts[idx] += e;
        }
    }
    if (tmp > 1) {            /* tmp is prime */
        int p = (int)tmp;
        int idx = prime_index_of[p];
        if (counts[idx] == 0) usedIdx[usedIdx_len++] = idx;
        counts[idx] += 1;
    }

    /* process indices in increasing order (using counts list) */
    int pos = 0;              /* position in the sorted multiset, 0‑based */
    for (size_t ui = 0; ui < usedIdx_len; ++ui) {
        int idx = usedIdx[ui];
        int c = counts[idx];
        for (int i = 0; i < c; ++i) {
            bool numerator = (pos % 2 == 0);
            int t = idx;     /* factor t (the index) */
            for (size_t pj = 0; pj < prime_cnt; ++pj) {
                uint32_t sp = primes[pj];
                uint64_t sp_sq = (uint64_t)sp * (uint64_t)sp;
                if (sp_sq > (uint64_t)t) break;
                if (t % sp == 0) {
                    int e = 0;
                    while (t % sp == 0) { t /= sp; ++e; }
                    if (numerator) {
                        if (diff[sp] == 0) usedDiff[usedDiff_len++] = sp;
                        diff[sp] += e;
                    } else {
                        if (diff[sp] == 0) usedDiff[usedDiff_len++] = sp;
                        diff[sp] -= e;
                    }
                }
            }
            if (t > 1) {                 /* remaining prime factor of t */
                int sp = t;
                if (numerator) {
                    if (diff[sp] == 0) usedDiff[usedDiff_len++] = sp;
                    diff[sp] += 1;
                } else {
                    if (diff[sp] == 0) usedDiff[usedDiff_len++] = sp;
                    diff[sp] -= 1;
                }
            }
            ++pos;
        }
    }

    /* test if all differences are non‑negative */
    bool ok = true;
    for (size_t i = 0; i < usedDiff_len; ++i) {
        int sp = usedDiff[i];
        if (diff[sp] < 0) { ok = false; break; }
    }

    /* reset auxiliary arrays for next call */
    for (size_t i = 0; i < usedDiff_len; ++i)
        diff[usedDiff[i]] = 0;
    usedDiff_len = 0;
    for (size_t i = 0; i < usedIdx_len; ++i)
        counts[usedIdx[i]] = 0;
    usedIdx_len = 0;

    return ok;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (!globals_initialized) {
        init_globals();
        globals_initialized = true;
    }
    idx_t cnt = 0;
    uint64_t m = 1;
    while (1) {
        if (check_property(m)) {
            ++cnt;
            if (cnt == n)
                return (seq_t)m;
        }
        ++m;
    }
}
