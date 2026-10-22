/*
 * A316914  For any k, the cumulative sum a(1) + a(2) + a(3) + ... + a(k)
 *          shares at least one digit of a(k). Lexicographic first sequence of
 *          positive integers without duplicate terms having this property.
 * https://oeis.org/A316914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 444, u32 n <= 9991, u64 n <= 9991,
 *            u128 n <= 9991, big any n
 * Helpers:   digit_mask_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A316914: lexicographically first sequence of distinct
 * positive integers such that for each k,
 *   S_k = a(1)+…+a(k)
 * shares at least one decimal digit with a(k).
 *
 * The algorithm builds the sequence incrementally.
 * For every next term it scans from 1 upward, skipping already
 * used numbers, and selects the first unused integer m for which
 * S_{k-1}+m shares a digit with m.
 * The sequence is cached in static arrays so that calls
 * with arbitrary index order are still O(n) after the first
 * expansion.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/digit_mask_seq_t.h"

/* Compare decimal digit masks of a and b */
static inline bool has_common_digit(seq_t a, seq_t b)
{
    return (digit_mask_seq_t(a) & digit_mask_seq_t(b)) != 0;
}

/* Global static cache */
static seq_t *seq = NULL;       /* terms */
static size_t seq_len = 0;
static size_t seq_cap = 0;

static bool *used = NULL;       /* used numbers, 1-indexed */
static size_t used_cap = 0;

static seq_t sum = 0;   /* cumulative sum of terms so far */

/* Ensure capacity of an array of type T. */
static void ensure_capacity(size_t *cap, void **ptr, size_t elem_size, size_t needed)
{
    if (*cap >= needed)
        return;
    size_t newCap = (*cap) ? (*cap) * 2 : 8;
    while (newCap < needed)
        newCap <<= 1;
    if (*cap == 0)
        *ptr = calloc(newCap, elem_size);
    else
        *ptr = realloc(*ptr, newCap * elem_size);
    /* Zero out any newly allocated portion */
    if (*cap < newCap)
        memset((char *)*ptr + (*cap) * elem_size, 0, (newCap - *cap) * elem_size);
    *cap = newCap;
}

/* Ensure used array capacity. All newly added elements are set to false. */
static void ensure_used_cap(size_t maxIndex)
{
    if (maxIndex < used_cap)
        return;
    ensure_capacity(&used_cap, (void **)&used, sizeof(bool), maxIndex + 1);
    /* The new portion is already zeroed by ensure_capacity. */
}

/* Ensure seq array capacity. */
static void ensure_seq_cap(size_t needed)
{
    if (seq_cap >= needed)
        return;
    ensure_capacity(&seq_cap, (void **)&seq, sizeof(seq_t), needed);
}

/* Generate terms until seq_len >= n. */
static void generate_up_to(idx_t n)
{
    while (seq_len < (size_t)n) {
        /* find smallest unused positive integer m such that
           (sum + m) shares a digit with m */
        idx_t m = 1;
        for (;;) {
            /* make sure we have enough storage for m */
            ensure_used_cap((size_t)m);

            if (!used[(size_t)m]) {
                seq_t candidateSum = sum + (seq_t)m;
                if (has_common_digit((seq_t)m, candidateSum)) {
                    /* accept m */
                    used[(size_t)m] = true;
                    ensure_seq_cap(seq_len + 1);
                    seq[seq_len++] = (seq_t)m;
                    sum = candidateSum;
                    break;
                }
            }
            ++m;
        }
    }
}

/* Main function requested by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined, but safe */
    generate_up_to(n);
    return seq[(size_t)n - 1];
}
