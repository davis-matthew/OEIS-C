/*
 * A328832  Numbers that are sums of distinct primorial numbers, A002110, and
 *          do not have a factor of the form p^p.
 * https://oeis.org/A328832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 351, u64 n <= 9718, u128 n <= 9718,
 *            big any n
 * Helpers:   big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A328832: Numbers that are sums of distinct primorial numbers
 * and do not have a factor of the form p^p (p prime).  The function
 * returns the n‑th term (1‑based index).  The implementation
 * precomputes all subset sums of the first 16 primorials (including 0# = 1),
 * filters out sums divisible by any p^p, sorts and deduplicates the
 * remaining numbers, and stores them in static memory so that
 * subsequent calls are O(1).  The algorithm runs in well below 2 s
 * for all n up to 10 000. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/big_pow.h"

static void init_sequence(void);

static seq_t *seq_array = NULL;
static size_t seq_len   = 0;
static bool initialized = false;

seq_t a(idx_t n)
{
    if (!initialized) {
        init_sequence();
        initialized = true;
    }
    /* n is guaranteed to be >= 1 */
    return seq_array[n-1];
}

/* -------------------------------------------------------------------------- */
static int cmp_seq(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

/* -------------------------------------------------------------------------- */
static void init_sequence(void)
{
    /* step 1: generate primes up to 100 (more than enough for the first 16 primorials) */
    uint64_t *prime_list = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(100, &prime_list, &prime_cnt);
    if (prime_cnt == 0) return; /* should not happen */

    /* step 2: compute first 16 primorials (0# to 15#) */
    const size_t MAX_PRIM = 16;          /* 0# .. 15# */
    seq_t primorial[MAX_PRIM];
    primorial[0] = 1;                    /* 0# = 1 */
    size_t i;
    for (i = 1; i < MAX_PRIM && i-1 < prime_cnt; ++i) {
        primorial[i] = primorial[i-1] * (seq_t)prime_list[i-1];
    }
    size_t prim_cnt = i;                 /* actual count of primorials generated */

    /* step 3: generate all subset sums of these primorials (2^prim_cnt) */
    size_t total = (size_t)1 << prim_cnt;
    seq_t *sums = (seq_t *)malloc(total * sizeof(seq_t));
    if (!sums) return;
    sums[0] = 0;
    size_t cur_size = 1;
    for (i = 0; i < prim_cnt; ++i) {
        size_t cur = cur_size;
        for (size_t j = 0; j < cur; ++j) {
            sums[cur_size++] = sums[j] + primorial[i];
        }
    }

    /* step 4: compute list of p^p for primes up to the last used prime */
    seq_t *bad_powers = NULL;
    size_t bad_cnt = 0;
    if (prim_cnt > 1) { /* primes start at index 1 (2) */
        bad_powers = (seq_t *)malloc(prim_cnt * sizeof(seq_t));
        if (bad_powers) {
            for (size_t k = 1; k < prim_cnt; ++k) { /* primes from prime_list[0] */
                seq_t p = (seq_t)prime_list[k-1];
                bad_powers[bad_cnt++] = big_pow(p, p);   /* compute p^p */
            }
        }
    }

    /* step 5: filter sums (exclude 0) and exclude those divisible by any p^p */
    seq_t *valid = (seq_t *)malloc(total * sizeof(seq_t));
    size_t valid_cnt = 0;
    for (size_t j = 1; j < cur_size; ++j) { /* skip 0 */
        seq_t val = sums[j];
        bool bad = false;
        for (size_t b = 0; b < bad_cnt; ++b) {
            seq_t pp = bad_powers[b];
            if (pp > val) break;            /* powers are increasing */
            if (val % pp == 0) { bad = true; break; }
        }
        if (!bad) valid[valid_cnt++] = val;
    }
    free(sums);
    free(bad_powers);
    free(prime_list);

    /* step 6: sort and deduplicate */
    qsort(valid, valid_cnt, sizeof(seq_t), cmp_seq);
    size_t uniq = 0;
    for (size_t j = 0; j < valid_cnt; ++j) {
        if (j == 0 || valid[j] != valid[j-1]) {
            valid[uniq++] = valid[j];
        }
    }

    /* allocate final array */
    seq_array = (seq_t *)malloc(uniq * sizeof(seq_t));
    if (!seq_array) { free(valid); return; }
    for (size_t j = 0; j < uniq; ++j) seq_array[j] = valid[j];
    seq_len = uniq;

    free(valid);
}
