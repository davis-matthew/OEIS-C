/*
 * A338923  Every even term k of the sequence is the cumulative sum of the
 *          even digits used so far (the digits of k are included in the sum).
 * https://oeis.org/A338923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 139, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   sum_even_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest distinct sequence A338923
 * (even term k equals cumulative sum of all even decimal digits
 * used up to that point, including the digits of k itself).
 * Computed by incremental search.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/sum_even_digits_idx.h"

static seq_t *seq = NULL;          /* sequence values, 0‑based */
static size_t seq_cap = 0;         /* allocated capacity of seq   */
static size_t seq_len = 0;         /* number of terms stored      */

static uint8_t *used = NULL;      /* boolean array: value already in sequence? */
static size_t used_cap = 0;       /* capacity of used array                */

static seq_t even_digit_sum_total = 0;   /* cumulative sum of even digits so far */

/* Ensure seq has room for at least 'needed' elements */
static inline void ensure_seq_cap(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 4;
    while (new_cap < needed) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Ensure used has room for at least 'needed' indices (0 .. needed-1) */
static inline void ensure_used_cap(idx_t needed)
{
    if (needed <= used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 8;
    while (new_cap < needed) new_cap <<= 1;
    used = (uint8_t *)realloc(used, new_cap * sizeof(uint8_t));
    /* zero newly allocated part */
    memset(used + used_cap, 0, (new_cap - used_cap) * sizeof(uint8_t));
    used_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined below offset, return 0 */

    /* Generate terms up to n (1‑based) */
    while (seq_len < (size_t)n) {
        idx_t candidate = 0;
        /* Find the smallest unused integer that satisfies the
         * even‑term property (if it is even). */
        for ( ;; ++candidate ) {
            ensure_used_cap(candidate + 1);
            if (used[candidate]) continue;   /* already in sequence */

            seq_t e_contrib = sum_even_digits_idx(candidate);

            if ((candidate & 1U) == 0) {          /* candidate is even */
                if ( (seq_t)candidate != even_digit_sum_total + e_contrib )
                    continue;                    /* violates condition */
            }
            /* Accept this candidate */
            ensure_seq_cap(seq_len + 1);
            seq[seq_len++] = (seq_t)candidate;
            even_digit_sum_total += e_contrib;
            used[candidate] = 1;
            break;
        }
    }
    return seq[n - 1];   /* 0‑based indexing in seq array */
}
