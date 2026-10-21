/*
 * A285744  Lexicographically earliest sequence of distinct positive terms
 *          such that, for any n>0, n*a(n) has at least 5 distinct prime
 *          factors.
 * https://oeis.org/A285744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9985, u32 n <= 9985, u64 n <= 9985,
 *            u128 n <= 9985, big any n
 * Helpers:   count_distinct_primes, gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A285744 – lexicographically earliest permutation with
 *  the property that n*a(n) has at least 5 distinct prime factors.
 *
 *  The function a(idx_t n) returns the n‑th term (n≥1).
 *  It builds the permutation on demand, storing the results
 *  in static arrays.  Each number is paired with the smallest
 *  unused number that, together with the current index,
 *  gives a product having at least five distinct prime factors.
 *
 *  The algorithm uses the helper:
 *    int count_distinct_primes(uint64_t n);
 *  to obtain the number of distinct prime factors.
 *
 *  Complexity:  O(N) average for n up to about 10 000
 *  (the pair search is linear in the number of unused candidates,
 *  each candidate is examined at most once).
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/count_distinct_primes.h"
#include "../utils/gcd64.h"

/* ----------  State  ----------------------------------------------- */

static bool seq_inited = false;

/* permutation array a[0..] : a[0] unused, values are seq_t            */
static seq_t *perm_array = NULL;
static size_t perm_cap = 0;
static size_t perm_len = 0;

/* flag whether a number has already appeared as a value           */
static unsigned char *used_flag = NULL;
static size_t used_cap = 0;
static size_t used_len = 0;

/* cached distinct‑prime counts for all numbers that have been
 * factorised; 0 means “not computed yet” (except for n=0,1)     */
static int *prime_cnt = NULL;
static size_t prime_cnt_cap = 0;
static size_t prime_cnt_len = 0;

/* highest index that has been fully processed                      */
static idx_t processed_upto = 0;

/* ----------  Utility helpers -------------------------------------- */

static void ensure_perm_len(size_t idx)
{
    if (idx < perm_len) return;
    size_t new_cap = (perm_cap) ? (perm_cap << 1) : 64;
    while (new_cap <= idx) new_cap <<= 1;
    perm_array = (seq_t *)realloc(perm_array, new_cap * sizeof(seq_t));
    memset(perm_array + perm_len, 0, (new_cap - perm_len) * sizeof(seq_t));
    perm_len = perm_cap = new_cap;
}

static void ensure_used_len(size_t idx)
{
    if (idx < used_len) return;
    size_t new_cap = (used_cap) ? (used_cap << 1) : 512;
    while (new_cap <= idx) new_cap <<= 1;
    used_flag = (unsigned char *)realloc(used_flag, new_cap * sizeof(unsigned char));
    memset(used_flag + used_len, 0, (new_cap - used_len) * sizeof(unsigned char));
    used_len = used_cap = new_cap;
}

static void ensure_prime_cnt_len(size_t idx)
{
    if (idx < prime_cnt_len) return;
    size_t new_cap = (prime_cnt_cap) ? (prime_cnt_cap << 1) : 1024;
    while (new_cap <= idx) new_cap <<= 1;
    prime_cnt = (int *)realloc(prime_cnt, new_cap * sizeof(int));
    memset(prime_cnt + prime_cnt_len, 0, (new_cap - prime_cnt_len) * sizeof(int));
    prime_cnt_len = prime_cnt_cap = new_cap;
}

static inline int get_prime_cnt(uint64_t x)
{
    ensure_prime_cnt_len((size_t)x);
    if (x <= 1) return 0;
    int *p = &prime_cnt[(size_t)x];
    if (*p == 0) {
        *p = count_distinct_primes(x);
    }
    return *p;
}

/* 64‑bit gcd ------------------------------------------------------- */

/* Initialise static data once ------------------------------------------------ */
static void seq_init(void)
{
    if (seq_inited) return;
    seq_inited = true;

    /* allocate minimal initial capacities */
    perm_cap = perm_len = 64;
    perm_array = (seq_t *)calloc(perm_cap, sizeof(seq_t));

    used_cap = used_len = 512;
    used_flag = (unsigned char *)calloc(used_cap, sizeof(unsigned char));

    prime_cnt_cap = prime_cnt_len = 1024;
    prime_cnt = (int *)calloc(prime_cnt_cap, sizeof(int));
}

/* ----------  Main function ----------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety – offset is 1 */
    seq_init();

    /* ensure arrays large enough for requested index */
    ensure_perm_len((size_t)n);
    ensure_used_len((size_t)n);
    ensure_prime_cnt_len((size_t)n);

    /* return previously computed value */
    if (perm_array[(size_t)n] != 0) return perm_array[(size_t)n];

    /* process all indices up to n that are still unpaired */
    for (idx_t i = processed_upto + 1; i <= n; ++i) {
        size_t idx = (size_t)i;
        ensure_perm_len(idx);
        ensure_used_len(idx);
        ensure_prime_cnt_len(idx);

        if (used_flag[idx]) continue;   /* already paired elsewhere */

        uint64_t partner = 0;
        int cnt_i = get_prime_cnt((uint64_t)i);

        if (cnt_i >= 5) {
            /* any unused number works – find the smallest unused */
            partner = 1;
            while (true) {
                ensure_used_len((size_t)partner);
                if (!used_flag[(size_t)partner]) break;
                ++partner;
            }
        } else {
            /* search for smallest unused candidate satisfying
             * union of prime factors ≥ 5 */
            partner = 1;
            while (true) {
                ensure_used_len((size_t)partner);
                if (used_flag[(size_t)partner]) { ++partner; continue; }

                int cnt_p = get_prime_cnt((uint64_t)partner);
                uint64_t g = gcd64((uint64_t)i, (uint64_t)partner);
                int cnt_g = (g > 1) ? get_prime_cnt(g) : 0;
                int union_cnt = cnt_i + cnt_p - cnt_g;

                if (union_cnt >= 5) break;
                ++partner;
            }
        }

        /* record the pair */
        size_t pidx = (size_t)partner;
        ensure_perm_len(pidx);
        ensure_used_len(pidx);
        ensure_prime_cnt_len(pidx);

        perm_array[idx] = (seq_t)partner;
        perm_array[pidx] = (seq_t)i;
        used_flag[idx] = 1;
        used_flag[pidx] = 1;
    }

    processed_upto = n;
    return perm_array[(size_t)n];
}
