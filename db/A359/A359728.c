/*
 * A359728  a(1) = 1; a(n) is the smallest positive number not among the first
 *          k terms where k is the number of times a(n-1) has occurred.
 * https://oeis.org/A359728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A359728 – a(1) = 1.
   a(n) is the smallest positive integer not among the first k terms,
   where k is the number of times a(n‑1) has occurred.  Implements a
   simple dynamic generation with static arrays (good for C11 and C++17). */

#include <stddef.h>
#include <stdbool.h>

#define MAX_TERMS 20000             /* safe upper bound for n up to 10000 */

static seq_t  seq_cache[MAX_TERMS];   /* stored sequence terms */
static size_t seq_len = 0;            /* number of terms currently stored */

/* Frequency table – each entry holds a distinct value and its count. */
struct freq_entry {
    seq_t  val;
    idx_t  cnt;
};
static struct freq_entry freq_tbl[MAX_TERMS];
static size_t freq_len = 0;

/* Return a pointer to the frequency entry for v, or NULL if not present. */
static struct freq_entry *freq_find(const seq_t *v)
{
    for (size_t i = 0; i < freq_len; ++i)
        if (freq_tbl[i].val == *v)
            return &freq_tbl[i];
    return NULL;
}

/* Increment the occurrence count for value v, adding a new entry if needed. */
static void freq_inc(const seq_t *v)
{
    struct freq_entry *e = freq_find(v);
    if (e) {
        ++e->cnt;
    } else {
        /* New distinct value – assuming freq_len < MAX_TERMS. */
        freq_tbl[freq_len].val = *v;
        freq_tbl[freq_len].cnt = 1;
        ++freq_len;
    }
}

/* Compute and store a(n) if not already present. */
static void compute_until(idx_t n)
{
    while (seq_len < (size_t)n) {
        if (seq_len == 0) {          /* a(1) = 1 */
            seq_cache[0] = (seq_t)1;
            freq_inc(&seq_cache[0]);
            ++seq_len;
            continue;
        }

        seq_t prev = seq_cache[seq_len - 1];
        struct freq_entry *e_prev = freq_find(&prev);
        idx_t k = (e_prev ? e_prev->cnt : 0);   /* number of times prev has appeared */

        /* Find the smallest positive integer not in the first k terms. */
        seq_t m = (seq_t)1;
        while (true) {
            bool present = false;
            for (idx_t i = 0; i < k; ++i) {
                if (seq_cache[i] == m) {
                    present = true;
                    break;
                }
            }
            if (!present)
                break;
            ++m;
        }

        /* Store the new term. */
        seq_cache[seq_len] = m;
        freq_inc(&m);
        ++seq_len;
    }
}

/* Public function required by OEIS. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* behaviour for n < 1 is arbitrary */

    compute_until(n);
    return seq_cache[n - 1];
}
