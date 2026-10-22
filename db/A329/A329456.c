/*
 * A329456  For any n >= 0, exactly four sums a(n+i) + a(n+j) are prime, for 0
 *          <= i < j <= 4: lexicographically earliest such sequence of
 *          distinct nonnegative integers.
 * https://oeis.org/A329456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined recursively by lexicographic greedy search:
 *            a(0)=0, a(1)=1, a(2)=2, a(3)=3 and for n>=4, a(n) is the
 *            smallest unused non‑negative integer that makes exactly four
 *            primes among the 10 pairwise sums of the five consecutive terms
 *            a(n-4)…a(n). Sequence values are distinct. The function...
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime.h"         /* provides bool is_prime(idx_t) */

/* global static tables */
static seq_t *seq            = NULL;      /* stored sequence values */
static idx_t seq_size        = 0;          /* number of values stored   */
static size_t seq_cap        = 0;          /* allocated capacity        */
static bool *used            = NULL;      /* flags: true if number used*/
static size_t used_cap       = 0;          /* capacity of used array    */
static bool initialized      = false;      /* initialisation flag       */

/* helper for growing the used array to at least 'need' entries */
static void grow_used(size_t need)
{
    if (need < used_cap) return;
    size_t new_cap = need * 2 + 100;
    bool *new_arr = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!new_arr) exit(1);                     /* unrecoverable: out of memory */
    memset(new_arr + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used = new_arr;
    used_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* --------------------------------------------------------------------- */
    /* initialise static data on first invocation -------------------------- */
    if (!initialized) {
        /* capacity for the sequence array (will grow as needed) */
        seq_cap  = 32;
        seq      = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) exit(1);

        /* used array: start with small size, grow when required */
        used_cap = 128;
        used     = (bool *)calloc(used_cap, sizeof(bool));
        if (!used) exit(1);

        /* first four terms are fixed: 0,1,2,3 */
        seq[0] = (seq_t)0;
        seq[1] = (seq_t)1;
        seq[2] = (seq_t)2;
        seq[3] = (seq_t)3;
        seq_size = 4;

        used[0] = used[1] = used[2] = used[3] = true;
        initialized = true;
    }

    /* enlarge the sequence array if a larger index is requested */
    if (n + 1 > seq_cap) {
        seq_cap = (n + 1) * 2;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        if (!seq) exit(1);
    }

    /* --------------------------------------------------------------------- */
    /* produce terms up to and including a(n) ------------------------------- */
    while (seq_size <= n) {
        /* Gather the four previous terms that form the current 5‑term block */
        seq_t prev[4];
        prev[0] = seq[seq_size - 4];
        prev[1] = seq[seq_size - 3];
        prev[2] = seq[seq_size - 2];
        prev[3] = seq[seq_size - 1];

        /* Compute the number of primes among the 6 previous‑previous sums */
        int base_prime = 0;
        for (int i = 0; i < 4; ++i) {
            for (int j = i + 1; j < 4; ++j) {
                if (is_prime((idx_t)(prev[i] + prev[j]))) ++base_prime;
            }
        }
        int need = 4 - base_prime;          /* primes the candidate must contribute */

        /* Search for the smallest unused candidate that satisfies the condition */
        idx_t cand = 0;
        for (;;) {
            if (cand >= used_cap) grow_used((size_t)cand + 1);
            if (used[(size_t)cand]) { ++cand; continue; }

            int cnt = 0;
            for (int i = 0; i < 4; ++i) {
                if (is_prime((idx_t)(prev[i] + (idx_t)cand))) {
                    ++cnt;
                    if (cnt > need) break;   /* early exit if too many primes */
                }
            }
            if (cnt == need) {                /* candidate is valid */
                seq[seq_size++] = (seq_t)cand;
                if (cand >= used_cap) grow_used((size_t)cand + 1);
                used[(size_t)cand] = true;
                break;                       /* proceed to next term */
            }
            ++cand;
        }
    }

    /* --------------------------------------------------------------------- */
    return seq[n];
}
