/*
 * A254873  Recamán [divide, -, +, *]-sequence with seed 14 and step 2.
 * https://oeis.org/A254873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, hear, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Recurrence: a(1) = 14
   a(n) = first of {a(n-1)/2, a(n-1)-2, a(n-1)+2, a(n-1)*2}
          that yields a positive integer not already present in the
          sequence.  Operations are attempted in this priority order:
          division, subtraction, addition, multiplication.
*/
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t *seq      = NULL;   /* storage for the sequence values   */
static size_t seq_len  = 0;       /* number of terms currently stored   */
static size_t seq_cap  = 0;       /* allocated capacity                 */

/* Ensure the internal array has room for at least 'needed' elements */
static void ensure_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Linear search: does 'val' already appear in the sequence? */
static bool already_used(seq_t val)
{
    for (size_t i = 0; i < seq_len; ++i)
        if (seq[i] == val) return true;
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* out of range for this sequence */

    /* Seed the sequence if it has not yet been initialised */
    if (seq_len == 0) {
        ensure_capacity(1);
        seq[0] = (seq_t)14;
        seq_len = 1;
    }

    /* Compute terms up to the requested index */
    while (seq_len < (size_t)n) {
        seq_t cur = seq[seq_len - 1];
        seq_t cand = 0;
        bool found = false;
        const seq_t two = (seq_t)2;

        /* Division: only if divisible by 2 and result positive */
        if (cur % two == (seq_t)0) {
            cand = cur / two;
            if (cand > (seq_t)0 && !already_used(cand)) {
                found = true;
            }
        }

        /* Subtraction: only if cur > 2 to avoid underflow */
        if (!found && cur > two) {
            cand = cur - two;
            if (cand > (seq_t)0 && !already_used(cand)) {
                found = true;
            }
        }

        /* Addition: always positive */
        if (!found) {
            cand = cur + two;
            if (!already_used(cand)) {
                found = true;
            }
        }

        /* Multiplication: fallback */
        if (!found) {
            cand = cur * two;
            if (!already_used(cand)) {
                found = true;
            }
        }

        /* Append the chosen candidate */
        ensure_capacity(seq_len + 1);
        seq[seq_len] = cand;
        ++seq_len;
    }

    return seq[n - 1];
}
