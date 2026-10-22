/*
 * A307023  Lexicographically earliest sequence starting with a(1) = 1 and
 *          a(2) = 2 such that two consecutive terms of opposite parity are
 *          followed by their sum; else (same parity), by the smallest term
 *          not yet in the sequence.
 * https://oeis.org/A307023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 9830, u32 n <= 9433, u64 n <= 9433,
 *            u128 n <= 9433, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest sequence with a(1)=1,a(2)=2
   Rule: if a(n-2),a(n-1) have opposite parity then a(n)=a(n-2)+a(n-1),
   otherwise a(n) = the smallest positive integer not yet in the sequence. */

#include <stdlib.h>
#include <stdbool.h>

/* static storage for the sequence and helper data */
static seq_t *seq = NULL;          /* array of computed terms */
static size_t seq_len = 0;          /* number of terms stored */
static size_t seq_cap = 0;          /* capacity of the array */
static seq_t next_missing = 1;      /* next candidate for the smallest missing */

/* Linear search to test presence of a value in seq[0..len-1] */
static bool contains(const seq_t *arr, size_t len, seq_t val)
{
    for (size_t i = 0; i < len; ++i)
        if (arr[i] == val)
            return true;
    return false;
}

/* Find the next unused positive integer (smallest missing). */
static seq_t find_next_missing(void)
{
    while (contains(seq, seq_len, next_missing))
        ++next_missing;
    seq_t val = next_missing;
    ++next_missing;
    return val;
}

/* Append a term to the static sequence array, expanding capacity if needed. */
static void append(seq_t val)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 4;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
    }
    seq[seq_len++] = val;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    /* initialise with the first two terms if not already done */
    if (seq_len == 0) {
        seq_cap = 4;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        append((seq_t)1);
        append((seq_t)2);
        /* next_missing starts at 1; it will be advanced when needed */
    }

    /* compute missing terms up to n */
    for (idx_t i = seq_len + 1; i <= n; ++i) {
        /* indices are 1‑based: a(i-2) = seq[i-3], a(i-1) = seq[i-2] */
        seq_t prevprev = seq[i - 3];
        seq_t prev     = seq[i - 2];
        seq_t next;
        /* opposite parity when XOR of bits is odd */
        if (((prevprev ^ prev) & 1) != 0)
            next = prevprev + prev;          /* forced sum */
        else
            next = find_next_missing();      /* smallest unused integer */
        append(next);
    }

    return seq[n - 1];
}
