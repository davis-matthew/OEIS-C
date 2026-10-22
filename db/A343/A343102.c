/*
 * A343102  a(n) is the sum of the number of times the digits in n (without
 *          repetition) have appeared in the sequence.
 * https://oeis.org/A343102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A343102: a(n) = Σ_{d∈distinct digits of n} count[d],
   where count[d] is the number of times digit d has appeared in the
   sequence a(0),a(1),…,a(n‑1).  Initial value: a(0)=0. */

/*  The implementation keeps the whole sequence in a dynamically
    allocated array and updates a digit frequency table as it goes.
    Subsequent calls can return any previously computed value in
    O(1) time, and new values are generated in linear time. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* static state ----------------------------------------------------- */
static seq_t *seq_vals = NULL;     /* array of sequence values       */
static size_t  seq_cap  = 0;       /* capacity of seq_vals           */
static idx_t   next_compute = 0;   /* next index to compute          */
static seq_t   digit_cnt[10] = {0};/* cumulative digit occurrences   */

/* ensure that seq_vals can hold indices 0 … n --------------------- */
static void ensure_seq_size(idx_t n)
{
    if (seq_cap <= (size_t)n) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(seq_vals, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_vals = tmp;
        seq_cap  = new_cap;
    }
}

/* sequence entry ---------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialise on first call */
    if (next_compute == 0) {
        ensure_seq_size(0);
        seq_vals[0] = 0;         /* a(0) = 0 */
        digit_cnt[0] = 1;        /* the digit 0 from a(0) */
        next_compute = 1;
    }

    /* already computed? */
    if (n < next_compute) {
        return seq_vals[n];
    }

    /* compute missing values up to n */
    for (idx_t i = next_compute; i <= n; ++i) {
        seq_t s = 0;
        idx_t tmp = i;
        bool seen[10] = {0};

        /* sum counts of distinct digits of i */
        if (tmp == 0) {
            seen[0] = true;
            s += digit_cnt[0];
        } else {
            while (tmp > 0) {
                int d = (int)(tmp % 10);
                if (!seen[d]) {
                    s += digit_cnt[d];
                    seen[d] = true;
                }
                tmp /= 10;
            }
        }

        ensure_seq_size(i);
        seq_vals[i] = s;

        /* update digit counts with the digits of s (with multiplicity) */
        if (s == 0) {
            digit_cnt[0] += 1;
        } else {
            seq_t tmp_s = s;
            while (tmp_s > 0) {
                int d = (int)(tmp_s % 10);
                digit_cnt[d] += 1;
                tmp_s /= 10;
            }
        }
    }

    next_compute = n + 1;
    return seq_vals[n];
}
