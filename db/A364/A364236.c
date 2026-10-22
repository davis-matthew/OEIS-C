/*
 * A364236  a(1) = 1. For n > 1, if a(n-1) is a novel term, a(n) = d(a(n-1)),
 *          else if a(n-1) is a repeat term seen k (>1) times, a(n) = a(n-1) +
 *          d(k-1), where d is the divisor counting function A000005.
 * https://oeis.org/A364236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(1)=1; for n>1, let k be the number of occurrences of a(n‑1)
 *            seen so far. If k==1 (novel term) then a(n)=d(a(n‑1)); else
 *            a(n)=a(n‑1)+d(k‑1)
 * Range:     u8 n <= 8388, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
/* Function for computing the divisor count of a 64‑bit integer
   (a helper used by the recurrence). */
static inline uint64_t divisor_count_u64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t cnt = 1;
    uint64_t d   = 2;
    while (d * d <= n) {
        if (n % d == 0) {
            unsigned exp = 0;
            while (n % d == 0) {
                n /= d;
                ++exp;
            }
            cnt *= (exp + 1);
        }
        ++d;
    }
    if (n > 1) cnt *= 2;
    return cnt;
}

/* Use the seq_ helper for proper allocation of arbitrary‑precision types. */
#include "../utils/seq_realloc.h"


seq_t a(idx_t n)
{
    static seq_t *seq      = NULL;   /* array of already computed terms */
    static size_t seq_cap  = 0;      /* allocated capacity of seq */
    static size_t seq_len  = 0;      /* number of terms currently stored */

    /* Initialise on first call: store a(1)=1. */
    if (seq_len == 0) {
        size_t cap = 16;
        seq = seq_realloc(NULL, 0, cap);
        seq_cap = cap;
        seq[0] = (seq_t)1;
        seq_len = 1;
    }

    /* Grow the sequence until we have at least n terms. */
    while (seq_len < (size_t)n) {
        seq_t prev = seq[seq_len - 1];   /* a(n‑1) for the next iter */

        /* Count how many times prev has appeared so far. */
        unsigned k = 0;
        for (size_t i = 0; i < seq_len; ++i)
            if (seq[i] == prev) ++k;

        seq_t next;
        if (k == 1) { /* novel term */
            next = (seq_t)divisor_count_u64((uint64_t)prev);
        } else {      /* repeat term */
            next = (seq_t)((uint64_t)prev + divisor_count_u64((uint64_t)(k - 1)));
        }

        /* Ensure capacity and append next. */
        if (seq_len + 1 > seq_cap) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 16;
            seq = seq_realloc(seq, seq_cap, new_cap);
            seq_cap = new_cap;
        }
        seq[seq_len++] = next;
    }

    return seq[(size_t)n - 1];
}
