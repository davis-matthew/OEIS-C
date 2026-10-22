/*
 * A356188  a(1)=1; for n > 1, if a(n-1) is prime then a(n) = the smallest
 *          number not yet in the sequence. Otherwise a(n) = a(n-1) + n - 1.
 * https://oeis.org/A356188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1. For n>1, if a(n-1) is prime then
   a(n)=the smallest positive integer not yet in the sequence;
   otherwise a(n)=a(n-1)+(n-1). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/is_prime_big.h"

/* Static storage for the sequence (1‑based) and auxiliary data. */
static seq_t *seq = NULL;          /* sequence values, seq[0] unused */
static size_t seq_len = 0;         /* highest index computed so far */
static size_t seq_cap = 0;         /* current capacity of seq array */

/* Bitset of numbers that have appeared in the sequence. */
static unsigned long *used_bits = NULL;  /* array of 64‑bit words */
static size_t used_bits_len = 0;         /* number of words allocated */
static uint64_t smallest_unused = 1;     /* smallest integer not yet used */

/* Ensure seq has capacity for at least 'needed' indices (1‑based). */
static void ensure_seq_cap(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? (seq_cap << 1) : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    seq = (seq_t*)realloc(seq, (new_cap + 1) * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Ensure used_bits can index at least 'val'. */
static void ensure_used_bits(uint64_t val)
{
    size_t needed_words = (val >> 6) + 1;
    if (needed_words <= used_bits_len)
        return;
    size_t new_len = used_bits_len ? (used_bits_len << 1) : 8;
    if (new_len < needed_words)
        new_len = needed_words;
    used_bits = (unsigned long*)realloc(used_bits, new_len * sizeof(unsigned long));
    for (size_t i = used_bits_len; i < new_len; ++i)
        used_bits[i] = 0;
    used_bits_len = new_len;
}

/* Set the bit for 'val' in the bitset. */
static void set_used(uint64_t val)
{
    ensure_used_bits(val);
    size_t word = val >> 6;
    unsigned long mask = 1ULL << (val & 63);
    used_bits[word] |= mask;
}

/* Test whether 'val' has already been used. */
static bool is_used(uint64_t val)
{
    if (val >> 6 >= used_bits_len)
        return false;
    size_t word = val >> 6;
    unsigned long mask = 1ULL << (val & 63);
    return (used_bits[word] & mask) != 0;
}

/* Advance `smallest_unused` until it points at an unused value. */
static void update_smallest_unused(void)
{
    while (is_used(smallest_unused))
        ++smallest_unused;
}

/* Compute a(n) for any n >= 1.  The function is memoised and
   independent of call order. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined offset, return 0 */

    /* Ensure we can store up to n. */
    ensure_seq_cap((size_t)n);

    /* Initialise the sequence if empty. */
    if (seq_len == 0) {
        /* a(1) = 1 */
        seq[1] = (seq_t)1;
        seq_len = 1;
        set_used(1);
        update_smallest_unused(); /* smallest_unused should be 2 now */
    }

    /* Compute missing terms up to n. */
    for (size_t i = seq_len + 1; i <= (size_t)n; ++i) {
        seq_t prev = seq[i - 1];
        seq_t val;

        /* If previous term is prime, pick smallest unused integer. */
        if (is_prime_big(prev)) {
            val = (seq_t)smallest_unused;
        } else {
            /* Otherwise, add n-1 to the previous value. */
            val = prev + (seq_t)(i - 1);
        }

        seq[i] = val;

        /* Mark val as used.  Only values that fit in 64 bits are tracked. */
        /* The cast is safe because the sequence grows slowly for n up to 10000. */
        /* If val overflows UINT64_MAX, we simply skip tracking it. */
        {
            uint64_t uval = (uint64_t)val;
            if ((seq_t)uval == val)            /* no overflow */
                set_used(uval);
        }

        /* If we just used the current smallest unused number,
           advance to the next unused integer. */
        if ((uint64_t)val == smallest_unused)
            update_smallest_unused();
    }

    seq_len = (size_t)n;
    return seq[(size_t)n];
}
