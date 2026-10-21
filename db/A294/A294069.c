/*
 * A294069  The smallest digit of a(n+1) is strictly smaller than the largest
 *          digit of a(n).
 * https://oeis.org/A294069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 253, u16 n <= 8720, u32 n <= 8720, u64 n <= 8720,
 *            u128 n <= 8720, big any n
 * Helpers:   digit_frequencies, smallest_digit_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A294069 – Greedy construction.
   a(1) = 1.
   For n > 1, a(n) is the smallest positive integer not yet used
   whose smallest decimal digit is strictly smaller than the largest
   decimal digit of a(n‑1).  The sequence is a permutation of the
   positive integers.                                                            */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_frequencies.h"
#include "../utils/smallest_digit_idx.h"

static seq_t   *seq_arr = NULL;     /* stored values a(1)..a(current) */
static size_t   seq_cap = 0;
static size_t   seq_len = 0;        /* number of values already stored */

static bool    *used = NULL;       /* used[i] == true iff integer i already occurred   */
static size_t   used_cap = 0;      /* capacity of 'used' array (indices 0..used_cap-1) */

static idx_t    next_unused[10];   /* next unused integer for each smallest‑digit bucket */
static bool     initialized[10] = {false};

static void ensure_used_cap(idx_t need)
{
    if (need < used_cap)
        return;
    size_t new_cap = used_cap > 0 ? used_cap << 1 : 1024;
    while ((idx_t)new_cap <= need)
        new_cap <<= 1;
    bool *tmp = (bool*)realloc(used, new_cap * sizeof(bool));
    if (!tmp)
        abort();
    for (size_t i = used_cap; i < new_cap; ++i)
        tmp[i] = false;
    used = tmp;
    used_cap = new_cap;
}

static void set_used(idx_t x)
{
    if (x >= used_cap)
        ensure_used_cap(x);
    used[x] = true;
}

static bool is_used(idx_t x)
{
    return x < used_cap && used[x];
}

/* Largest decimal digit of a seq_t value using the digit frequency helper. */
static int largest_digit_seq(seq_t x)
{
    unsigned int freq[10] = {0};
    digit_frequencies(x, freq);
    for (int d = 9; d >= 0; --d)
        if (freq[d])
            return d;
    return 0;                   /* never reached for a positive integer */
}

/* Grow seq_arr capacity to at least n+1 elements. */
static void ensure_seq_cap(size_t n)
{
    if (n < seq_cap)
        return;
    size_t new_cap = seq_cap > 0 ? seq_cap << 1 : 16;
    while (new_cap <= n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(seq_arr, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    seq_arr = tmp;
    seq_cap = new_cap;
}

/* Find the next unused integer whose smallest digit equals 'd'. */
static idx_t find_next_in_bucket(int d)
{
    idx_t cand = 1;
    for (;;) {
        if (cand >= used_cap)
            ensure_used_cap(cand);
        if (!used[cand] && smallest_digit_idx(cand) == d) {
            next_unused[d] = cand;
            initialized[d] = true;
            return cand;
        }
        ++cand;
    }
}

/* Main function. */
seq_t a(idx_t n)
{
    if (n == 0)        /* undefined, but return 0 for safety */
        return 0;

    /* Expand sequence until n is available. */
    while (seq_len < n) {
        if (seq_len == 0) {          /* first term */
            seq_t first = 1;
            ensure_seq_cap(0);
            seq_arr[0] = first;
            seq_len = 1;
            set_used(1);
            continue;
        }

        seq_t prev = seq_arr[seq_len - 1];
        int Lprev = largest_digit_seq(prev);

        /* Find smallest unused number whose smallest digit < Lprev. */
        idx_t candidate = (idx_t)-1;
        int selected_bucket = -1;
        for (int d = 0; d < Lprev; ++d) {
            if (!initialized[d])
                find_next_in_bucket(d);
            idx_t cand = next_unused[d];
            if (cand < candidate) {
                candidate = cand;
                selected_bucket = d;
            }
        }

        /* Append candidate to sequence. */
        ensure_seq_cap(seq_len);
        seq_arr[seq_len] = (seq_t)candidate;
        ++seq_len;
        set_used(candidate);

        /* Update the bucket from which the candidate was taken. */
        idx_t next = candidate + 1;
        for (;;) {
            if (next >= used_cap)
                ensure_used_cap(next);
            if (!used[next] && smallest_digit_idx(next) == selected_bucket) {
                next_unused[selected_bucket] = next;
                break;
            }
            ++next;
        }
    }

    return seq_arr[n - 1];
}
