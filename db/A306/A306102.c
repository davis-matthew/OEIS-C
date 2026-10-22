/*
 * A306102  Numbers that are the difference of two positive squares in at
 *          least two ways.
 * https://oeis.org/A306102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer that can be written as a
 *            difference of two positive squares in at least two distinct
 *            ways. That is, a(n) is the n-th integer k such that A056924(k)
 *            >= 2. For odd k this occurs iff k has ≥4 distinct divisors, and
 *            if k is a square the inequality must become ≥5. For...
 * Range:     u8 n <= 18, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* To keep the implementation independent of any build‑specific
   helper, we perform prime factorisation by trial division using the
   helper floor_sqrt(seq_t).  This is O(√k) per candidate, which is
   fast enough for the required limits. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Count the number of divisors of a 64‑bit integer by trial division. */
static uint32_t divisor_count_uint64(uint64_t n)
{
    uint32_t dcnt = 1;
    uint64_t tmp = n;

    /* Handle factor 2 separately. */
    if ((tmp & 1ULL) == 0) {
        uint32_t exp = 0;
        while ((tmp & 1ULL) == 0) {
            tmp >>= 1;
            ++exp;
        }
        dcnt *= (exp + 1);
    }

    /* Factor the remaining odd part. */
    seq_t root = floor_sqrt((seq_t)tmp);
    for (idx_t p = 3; (seq_t)p <= root; p += 2) {
        if (tmp % p == 0) {
            uint32_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            dcnt *= (exp + 1);
            root = floor_sqrt((seq_t)tmp);
        }
    }

    /* If something remains, it is a prime > 1. */
    if (tmp > 1) {
        dcnt *= 2;
    }

    return dcnt;
}

/* Test whether a nonnegative integer x is a perfect square. */
static bool is_square_idx(idx_t x)
{
    seq_t r = floor_sqrt((seq_t)x);
    return r * r == (seq_t)x;
}

/* Static storage for memoisation of the sequence.  */
static seq_t *seq     = NULL;      /* array of terms                  */
static size_t seq_size = 0;        /* how many terms are stored       */
static size_t seq_cap  = 0;        /* allocated capacity               */
static idx_t last_candidate = 0;   /* last integer examined           */

/* Grow the sequence array if necessary. */
static void grow_seq_array(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!new_seq) exit(EXIT_FAILURE);
    seq = new_seq;
    seq_cap = new_cap;
}

/* Main function required by the problem.  */
seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but safe */

    while (seq_size < (size_t)n) {
        ++last_candidate;          /* examine next integer      */

        bool ok = false;

        if ((last_candidate & 1ULL) == 1) {               /* odd   */
            uint32_t d = divisor_count_uint64((uint64_t)last_candidate);
            if (is_square_idx(last_candidate))
                ok = d >= 5;
            else
                ok = d >= 4;
        } else if ((last_candidate & 3ULL) == 0) {         /* multiple of 4 */
            idx_t k = last_candidate >> 2;                /* n/4    */
            uint32_t d = divisor_count_uint64((uint64_t)k);
            if (is_square_idx(k))
                ok = d >= 5;
            else
                ok = d >= 4;
        } else {                                          /* n ≡ 2 (mod 4) */
            ok = false;
        }

        if (ok) {
            grow_seq_array(seq_size + 1);
            seq[seq_size++] = (seq_t)last_candidate;
        }
    }

    return seq[n - 1];
}
