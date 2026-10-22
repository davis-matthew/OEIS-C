/*
 * A357961  a(1) = 1, and for any n > 0, a(n+1) is the k-th positive number
 *          not yet in the sequence, where k is the Hamming weight of a(n).
 * https://oeis.org/A357961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 249, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   hamming_weight
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is defined recursively:
   a(1) = 1.
   For n > 0, let k = Hamming weight of a(n) (number of 1‑bits).
   Then a(n+1) is the k‑th positive integer not yet appearing in the
   sequence.  The function below computes a(n) for any n ≥ 1,
   memoising previously generated terms. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/hamming_weight.h"

/* Dynamic arrays used for memoisation. */
static seq_t  *seq   = NULL;   /* generated terms */
static size_t seq_cap = 0;     /* capacity of seq   */
static size_t seq_len = 0;     /* number of terms stored */

static bool *used = NULL;      /* bitmap of numbers already used */
static size_t used_cap = 0;    /* capacity of used array */

/* Ensure seq has at least 'needed' capacity. */
static void ensure_seq_capacity(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap*2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq = tmp;
    seq_cap = new_cap;
}

/* Ensure used array has at least 'needed' capacity. */
static void ensure_used_capacity(size_t needed)
{
    if (needed <= used_cap)
        return;
    size_t new_cap = used_cap ? used_cap*2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    /* initialise new entries to false */
    memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used = tmp;
    used_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Produce terms up to n if necessary. */
    for (idx_t i = seq_len + 1; i <= n; ++i)
    {
        seq_t next;
        if (i == 1) {
            next = 1;          /* a(1) = 1 */
        } else {
            /* k = Hamming weight of a(i-1). */
            seq_t prev = seq[i-2];
            unsigned k = hamming_weight(prev);

            /* Find the k‑th unused positive integer. */
            size_t pos = 0;    /* 0‑based index, value = pos+1 */
            unsigned cnt = 0;
            for (;;) {
                if (pos + 1 > used_cap)
                    ensure_used_capacity(pos + 1);
                if (!used[pos]) {
                    ++cnt;
                    if (cnt == k)
                        break;    /* found k‑th unused value */
                }
                ++pos;
            }
            next = (seq_t)(pos + 1);
        }

        /* Mark the found value as used. */
        size_t idx = (size_t)(next - 1);
        if (idx + 1 > used_cap)
            ensure_used_capacity(idx + 1);
        used[idx] = true;

        /* Append the value to the sequence. */
        ensure_seq_capacity(seq_len + 1);
        seq[seq_len] = next;
        ++seq_len;
    }

    return seq[n-1];
}
