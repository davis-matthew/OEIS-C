/*
 * A341817  If a(n) is odd, add to a(n) its odd digits and iterate; if a(n) is
 *          even, add to a(n) its even digits and iterate; if an iteration
 *          reproduces a term already in the sequence, don't do this iteration
 *          and extend the sequence with the smallest integer not yet present
 *          in the sequence.
 * https://oeis.org/A341817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 150, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1. For n>1, let a(n‑1)=x; if x is odd, y= sum of its odd digits;
   if x is even, y= sum of its even digits.  Candidate = x + y.
   If candidate is not yet in the sequence, a(n)=candidate;
   otherwise a(n)= the smallest positive integer not yet present. */

/* The helpers below are provided by the build system. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/digit_counts.h"   /* provides void digit_counts(seq_t, unsigned char[10]); */

/* Static cache of the permutation and bookkeeping */
static seq_t *seq_cache = NULL;      /* array of computed terms            */
static idx_t seq_len   = 0;          /* number of terms currently stored   */
static idx_t seq_cap   = 0;          /* allocated capacity of seq_cache    */
static seq_t next_unused = 1;        /* smallest integer not yet present   */

/* Return true if val occurs in the first len entries of seq_cache. */
static bool seq_contains(seq_t val, idx_t len)
{
    for (idx_t i = 0; i < len; ++i)
        if (seq_cache[i] == val)
            return true;
    return false;
}

/* Append val to seq_cache and advance next_unused. */
static void seq_append(seq_t val)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 128;
        seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
    }
    seq_cache[seq_len++] = val;

    /* Advance next_unused to the next integer not yet used */
    while (seq_contains(next_unused, seq_len))
        ++next_unused;
}

/* Sum the digits of x having the specified parity (true=odd). */
static seq_t digit_sum_parity(seq_t x, bool odd)
{
    unsigned char cnt[10];
    digit_counts(x, cnt);
    seq_t sum = 0;
    for (int d = 0; d < 10; ++d) {
        if ( ((d & 1) == odd) )
            sum += (seq_t)d * cnt[d];
    }
    return sum;
}

/* Grow the cache up to the requested index. */
static void generate_seq(idx_t target)
{
    while (seq_len < target) {
        if (seq_len == 0) {              /* first term */
            seq_append((seq_t)1);
            continue;
        }
        seq_t last = seq_cache[seq_len - 1];
        bool last_odd = (last % (seq_t)2 != 0);
        seq_t sum  = digit_sum_parity(last, last_odd);
        seq_t cand = last + sum;

        if (!seq_contains(cand, seq_len))
            seq_append(cand);
        else {
            seq_t unused = next_unused;
            while (seq_contains(unused, seq_len))
                ++unused;
            seq_append(unused);
        }
    }
}

/* Public interface required by the exercise. */
seq_t a(idx_t n)
{
    if (n == 0)                     /* undefined for 0, but return 0 safely */
        return (seq_t)0;
    generate_seq(n);
    return seq_cache[n - 1];
}
