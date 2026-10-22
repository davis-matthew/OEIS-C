/*
 * A367645  Lexicographically earliest sequence of distinct positive numbers
 *          with the property that the sequence formed by the pairs of digits
 *          adjacent to the commas between the terms equals the magnitude of
 *          the successive differences between the terms.
 * https://oeis.org/A367645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A367645 – lexicographically earliest sequence of distinct
   positive integers where the two‑digit number formed by the last digit
   of a(n‑1) and the leading digit of a(n) equals |a(n) – a(n‑1)|.
   The sequence terminates after the 12th term; a(n) = 0 for n > 12.   */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Forward declaration of seq_t – supplied by the build system.   */

/* Helper: the most significant (leading) decimal digit of x.  */
static unsigned leading_digit(seq_t x)
{
    /* seq_t supports '/' and '>=', returning a seq_t; loop until <10. */
    while (x >= 10) {
        x = x / 10;
    }
    return (unsigned)x;          /* fits in 0–9 */
}

/* Storage for the sequence; 12 terms plus room for extensions. */
static seq_t seq_storage[128];
static size_t seq_len = 0;

/* Linear check whether value v already appears in the sequence. */
static bool used_before(seq_t v)
{
    for (size_t i = 0; i < seq_len; ++i) {
        if (seq_storage[i] == v) {
            return true;
        }
    }
    return false;
}

/* Compute the next term of the sequence; return true if added. */
static bool extend_once(void)
{
    if (seq_len == 0) return false;            /* no elements yet */

    seq_t last = seq_storage[seq_len - 1];
    unsigned d1 = (unsigned)(last % 10);        /* last decimal digit */

    seq_t best = 0;      /* 0 cannot be a valid term (positive integer) */
    bool found = false;

    /* Try each possible leading digit d2 of the next term. */
    for (unsigned d2 = 1; d2 <= 9; ++d2) {
        seq_t diff = 10 * d1 + d2;              /* |diff| */

        /* Candidate 1: larger than last */
        seq_t cand = last + diff;
        if (cand > 0 && !used_before(cand) &&
            leading_digit(cand) == d2) {
            if (!found || cand < best) {
                best = cand;
                found = true;
            }
        }

        /* Candidate 2: smaller than last (if positive) */
        if (last > diff) {
            cand = last - diff;
            if (cand > 0 && !used_before(cand) &&
                leading_digit(cand) == d2) {
                if (!found || cand < best) {
                    best = cand;
                    found = true;
                }
            }
        }
    }

    if (!found) return false;                  /* no further term */

    seq_storage[seq_len++] = best;
    return true;
}

/* Public function: return the n-th term (1‑based).  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined for 0 */

    /* Initialise sequence on first call. */
    if (seq_len == 0) {
        seq_storage[0] = (seq_t)1;
        seq_len = 1;
    }

    /* Extend until we have at least n terms or can no longer extend. */
    while (seq_len < (size_t)n) {
        if (!extend_once()) break;             /* sequence terminated */
    }

    if (n <= seq_len) {
        return seq_storage[n - 1];
    }
    /* n beyond the finite sequence (12 terms). */
    return 0;
}
