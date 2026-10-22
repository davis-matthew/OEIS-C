/*
 * A330530  Lexicographically earliest sequence of distinct positive integers
 *          such that the product of two consecutive terms is always divisible
 *          by 4.
 * https://oeis.org/A330530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Greedy construction of the lexicographically earliest
   sequence of distinct positive integers such that
   a(n)*a(n+1) is always divisible by 4 (OEIS A330530). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* State kept between calls */
static seq_t *seq = NULL;       /* 1‑based sequence storage */
static size_t seq_cap = 0;      /* allocated capacity (>= seq_len+1) */
static idx_t seq_len = 0;       /* number of terms currently stored (>=1) */

/* Bitset marking numbers that have already appeared */
static uint8_t *used = NULL;         /* 1‑based bits */
static size_t used_bytes = 0;

/* Smallest unused integer in each required category */
static idx_t next_all   = 1;  /* smallest unused integer */
static idx_t next_even  = 2;  /* smallest unused even integer */
static idx_t next_mult4 = 4;  /* smallest unused multiple of 4 */

/* Ensure the bitset covers at least 'val' (bit index). */
static void ensure_used_capacity(idx_t val)
{
    while ((size_t)val >= used_bytes * 8) {
        size_t old_bytes = used_bytes;
        size_t new_bytes = used_bytes ? used_bytes * 2 : 1024;
        uint8_t *tmp = (uint8_t *)realloc(used, new_bytes);
        if (!tmp) abort();          /* allocation failed */
        used = tmp;
        memset(used + old_bytes, 0, new_bytes - old_bytes);
        used_bytes = new_bytes;
    }
}

/* Test whether number 'v' has already been used. */
static inline bool used_test(idx_t v)
{
    return (used[v >> 3] >> (v & 7)) & 1U;
}

/* Mark number 'v' as used. */
static inline void used_set(idx_t v)
{
    used[v >> 3] |= (1U << (v & 7));
}

/* Ensure the sequence array can hold at least index 'idx' (1‑based). */
static void ensure_seq_capacity(idx_t idx)
{
    if (idx >= (idx_t)seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        while (idx >= (idx_t)new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        seq = tmp;
        seq_cap = new_cap;
    }
}

/* Main function: return a(n). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n = 0 */

    /* Initialise on first call */
    if (seq_len == 0) {
        ensure_seq_capacity(1);
        seq[1] = (seq_t)1;
        seq_len = 1;

        ensure_used_capacity(1);
        used_set(1);

        /* Update smallest unused pointers */
        while (used_test(next_all)) ++next_all;
        while (used_test(next_even)) next_even += 2;
        while (used_test(next_mult4)) next_mult4 += 4;
    }

    /* Build up to required n (in any order of calls) */
    for (idx_t i = seq_len + 1; i <= n; ++i) {
        seq_t prev = seq[i - 1];
        idx_t candidate;

        /* Decide which category to pick from based on prev mod 4 */
        if ((prev & (seq_t)3) == (seq_t)0) {      /* prev ≡ 0 (mod 4) */
            candidate = next_all;
        } else if ((prev & (seq_t)1) != (seq_t)0) { /* prev odd */
            candidate = next_mult4;
        } else { /* prev ≡ 2 (mod 4) */
            candidate = next_even;
        }

        /* Ensure structures are large enough for this candidate */
        ensure_used_capacity(candidate);
        ensure_seq_capacity(i);

        /* Store the new term */
        seq[i] = (seq_t)candidate;
        seq_len = i;

        /* Mark it used */
        used_set(candidate);

        /* Advance the next_* pointers if they reached this candidate */
        if (candidate == next_all) {
            do { ++next_all; } while (used_test(next_all));
        }
        if (candidate == next_even) {
            do { next_even += 2; } while (used_test(next_even));
        }
        if (candidate == next_mult4) {
            do { next_mult4 += 4; } while (used_test(next_mult4));
        }
    }

    return seq[n];
}
