/*
 * A330674  Lexicographically earliest infinite sequence of distinct
 *          nonnegative terms such that a(n+1) is obtained by adding to a(n)
 *          the odd digits of a(n) and subtracting the even ones.
 * https://oeis.org/A330674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A330674: Lexicographically earliest sequence of distinct non‑negative
 * terms.
 *
 * Recurrence:
 *   a(1) = 1
 *   For n ≥ 1 let d₀, …, d_k be the decimal digits of a(n).
 *   Let O = Σ d_i for odd digits,   E = Σ d_i for even digits.
 *   Then a(n+1) = a(n) + O – E.
 *   If this candidate repeats a previously used value, the iteration is
 *   cancelled and a(n+1) is replaced by the smallest non‑negative integer
 *   not yet present in the sequence.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Static storage for the sequence. */
static seq_t *seq       = NULL;  /* array of computed terms */
static size_t seq_len   = 0;     /* number of terms stored */
static size_t seq_cap   = 0;     /* allocated capacity */

/* Compute the next candidate from value x. */
static inline seq_t next_value(seq_t x)
{
    seq_t odd  = 0;
    seq_t even = 0;
    seq_t y    = x;
    while (y > 0) {
        unsigned d = (unsigned)(y % 10);
        if (d & 1U) odd  += d;
        else        even += d;
        y /= 10;
    }
    return x + odd - even;
}

/* Ensure the array can hold at least `need` elements. */
static void grow_seq(size_t need)
{
    if (seq_len >= need) return;
    if (seq_cap == 0) seq_cap = 8;
    while (seq_cap < need) seq_cap <<= 1;
    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
}

/* Return true if `val` is already in the sequence. */
static bool used(seq_t val)
{
    for (size_t i = 0; i < seq_len; ++i) {
        if (seq[i] == val) return true;
    }
    return false;
}

/* Find the smallest non‑negative integer not yet used. */
static seq_t smallest_unused(void)
{
    seq_t nxt = 0;
    while (true) {
        bool found = false;
        for (size_t i = 0; i < seq_len; ++i) {
            if (seq[i] == nxt) {
                found = true;
                break;
            }
        }
        if (!found) return nxt;
        ++nxt;
    }
}

/* Generate the sequence up to index `n`. */
static void generate_upto(idx_t n)
{
    if (seq_len == 0) {
        grow_seq(1);
        seq[0] = (seq_t)1;
        seq_len = 1;
    }
    while (seq_len < (size_t)n) {
        seq_t last  = seq[seq_len - 1];
        seq_t cand  = next_value(last);
        if (!used(cand)) {
            grow_seq(seq_len + 1);
            seq[seq_len++] = cand;
        } else {
            seq_t nxt = smallest_unused();
            grow_seq(seq_len + 1);
            seq[seq_len++] = nxt;
        }
    }
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    generate_upto(n);
    return seq[(size_t)n - 1];
}
