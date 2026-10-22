/*
 * A329413  Lexicographically earliest sequence of distinct positive numbers
 *          such that among the pairwise sums of any five consecutive terms
 *          there are exactly two prime sums.
 * https://oeis.org/A329413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 253, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest permutation of the positive integers such that
   every block of five consecutive terms has exactly two prime pair‑sums.
   Constructed greedily: for each new term pick the smallest unused
   positive integer that keeps the last five terms satisfying the
   two‑prime rule. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_small.h"

/* static cache for the constructed sequence */
static seq_t  *seq      = NULL;   /* stored values */
static size_t  seq_cap  = 0;      /* allocated capacity of seq[] */
static size_t  seq_len  = 0;      /* number of terms already built */

/* bitset tracking which positive integers have appeared */
static uint8_t *used     = NULL;
static size_t   used_cap = 0;

/* Ensure that used[] can index up to 'need' (inclusive) */
static void ensure_used_capacity(size_t need)
{
    if (need <= used_cap) return;
    size_t cap = used_cap ? used_cap << 1 : 16;
    while (cap < need) cap <<= 1;
    uint8_t *tmp = (uint8_t*)realloc(used, cap);
    if (!tmp) abort();
    memset(tmp + used_cap, 0, cap - used_cap);
    used = tmp;
    used_cap = cap;
}

/* Ensure that seq[] can hold at least 'need' elements */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t cap = seq_cap ? seq_cap << 1 : 16;
    while (cap < need) cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(seq, cap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined value, harmless */

    /* lazy initialization */
    if (!seq) {
        seq_len = 5;
        seq_cap = 16;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)2;
        seq[2] = (seq_t)3;
        seq[3] = (seq_t)7;
        seq[4] = (seq_t)13;

        used_cap = 16;
        used = (uint8_t*)calloc(used_cap, sizeof(uint8_t));

        /* Mark the first five values as used */
        const seq_t init[5] = {1, 2, 3, 7, 13};
        for (int i = 0; i < 5; ++i) {
            size_t v = (size_t)init[i];
            if (v >= used_cap) ensure_used_capacity(v + 1);
            used[v] = 1;
        }
    }

    /* Build the sequence up to requested index */
    while (seq_len < (size_t)n) {
        idx_t cand = 1;
        bool found = false;
        while (!found) {
            /* Ensure used[] covers candidate */
            if ((size_t)cand >= used_cap) ensure_used_capacity((size_t)cand + 1);

            if (!used[(size_t)cand]) {
                /* Extract last four terms */
                seq_t a2 = seq[seq_len - 4];
                seq_t a3 = seq[seq_len - 3];
                seq_t a4 = seq[seq_len - 2];
                seq_t a5 = seq[seq_len - 1];

                /* Count prime pair-sums among these five terms */
                seq_t sums[10];
                sums[0] = a2 + a3; sums[1] = a2 + a4; sums[2] = a2 + a5;
                sums[3] = a3 + a4; sums[4] = a3 + a5; sums[5] = a4 + a5;
                sums[6] = a2 + (seq_t)cand;
                sums[7] = a3 + (seq_t)cand;
                sums[8] = a4 + (seq_t)cand;
                sums[9] = a5 + (seq_t)cand;

                int primes = 0;
                for (int i = 0; i < 10; ++i)
                    if (is_prime_small(sums[i])) ++primes;

                if (primes == 2) {
                    /* Accept this candidate */
                    seq_len++;
                    if (seq_len > seq_cap) ensure_seq_capacity(seq_len);
                    seq[seq_len - 1] = (seq_t)cand;
                    used[(size_t)cand] = 1;
                    found = true;
                }
            }
            ++cand;
        }
    }

    return seq[n - 1];   /* 1‑based indexing */
}
