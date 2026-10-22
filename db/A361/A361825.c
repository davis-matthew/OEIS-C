/*
 * A361825  a(1) = 1, a(2) = 2; for n > 2, a(n) is the smallest positive
 *          number that has not yet appeared that is a multiple of the
 *          smallest prime that does not divide a(n-2) + a(n-1).
 * https://oeis.org/A361825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 2; for n > 2, let s = a(n‑2)+a(n‑1). Let p be
 *            the smallest prime that does NOT divide s. Then a(n) is the
 *            smallest positive integer that has not yet appeared and is a
 *            multiple of p
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* ------------------------------------------------------------------ */
/*  Static cache for the sequence and a bitmap of used values.        */
static seq_t *seq = NULL;      /* dynamically allocated sequence */
static size_t seq_len  = 0;    /* number of terms computed so far */
static size_t seq_cap  = 0;    /* allocated capacity of seq */

static bool *used = NULL;      /* used[i] == true iff value i has appeared */
static size_t used_cap = 0;    /* allocated capacity of used array */

/* ------------------------------------------------------------------ */
/*  Grow seq array to hold at least 'need' elements.                   */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!seq) abort();
    seq_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Grow used array to have at least 'need' entries (indices 0..need-1).*/
static void ensure_used_cap(size_t need)
{
    if (need <= used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    bool *new_used = (bool *)calloc(new_cap, sizeof(bool));
    if (!new_used) abort();
    if (used) {
        memcpy(new_used, used, used_cap * sizeof(bool));
        free(used);
    }
    used = new_used;
    used_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Find the smallest prime p that does NOT divide 'sum'.             */
static uint64_t smallest_prime_not_dividing(seq_t sum)
{
    uint64_t u64sum = (uint64_t)sum;          /* sum fits in 64‑bit for all n up to 10000 */
    for (uint64_t p = 2; ; ++p) {
        if (!is_prime((idx_t)p)) continue;
        if (u64sum % p != 0) return p;
    }
}

/* ------------------------------------------------------------------ */
/*  Main function computing the sequence.                               */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for n==0, safe return */

    /* Initialise cache on first call */
    if (seq_len == 0) {
        ensure_seq_cap(2);
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)2;
        seq_len = 2;

        /* Mark 1 and 2 as used */
        ensure_used_cap(3);                  /* indices 0..2 inclusive */
        used[1] = true;
        used[2] = true;
    }

    /* Return cached value if already computed */
    if ((size_t)n <= seq_len) return seq[(size_t)n - 1];

    /* Compute terms up to n */
    for (size_t i = seq_len + 1; i <= (size_t)n; ++i) {
        /* Compute a(n-2)+a(n-1) */
        seq_t sum = seq[i - 3] + seq[i - 2];

        /* Smallest prime that does not divide that sum */
        uint64_t p = smallest_prime_not_dividing(sum);

        /* Find the smallest unused multiple of p */
        seq_t candidate = (seq_t)p;
        for (;;) {
            size_t idx = (size_t)candidate;          /* safe: candidate small */
            ensure_used_cap(idx + 1);
            if (!used[idx]) break;
            candidate += (seq_t)p;
        }

        /* Store the found term and mark it used */
        ensure_seq_cap(i);
        seq[i - 1] = candidate;
        used[(size_t)candidate] = true;
    }

    seq_len = (size_t)n;
    return seq[(size_t)n - 1];
}
