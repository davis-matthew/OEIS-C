/*
 * A255577  Numbers n not coprime to 10 such that there exists an integer k >
 *          1 where n^k contains n as its last digits in base 10.
 * https://oeis.org/A255577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 1972, u32 n <= 8867, u64 n <= 8867,
 *            u128 n <= 8867, big any n
 * Helpers:   val2, val5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Returns the nth term of OEIS A255577.
   The sequence consists of numbers not coprime to 10 that are
   k‑morphic, i.e. there exists an integer k > 1 such that
   n^k ends with the decimal representation of n.
   For such n the following condition holds:
        let d = number of decimal digits of n,
        a = v2(n) (exponent of 2),  b = v5(n) (exponent of 5).
   Then n is k‑morphic iff
        (a == 0 || a >= d)  &&  (b == 0 || b >= d).
   The function generates terms on demand, memoises them,
   and returns a(n) for every n ≥ 1.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/val2.h"
#include "../utils/val5.h"

/* Memoisation storage for the sequence. */
static seq_t *seq      = NULL;  /* seq[i] holds a(i+1) */
static size_t seq_cap  = 0;     /* allocated capacity */
static size_t seq_len  = 0;     /* number of computed terms */
static seq_t  candidate = 2;    /* next integer to test */

/* Grow the memoisation array to hold at least 'needed' terms. */
static void ensure_capacity(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failure: abort */
        exit(EXIT_FAILURE);
    }
    seq     = tmp;
    seq_cap = new_cap;
}

/* Count decimal digits of n. */
static idx_t digit_count(seq_t n)
{
    idx_t d = 0;
    if (n == 0)
        return 1;
    while (n > 0) {
        n /= 10;
        ++d;
    }
    return d;
}

/* Test whether n is k‑morphic (sub‑condition of the sequence). */
static bool is_k_morphic(seq_t n)
{
    if (n == 0)
        return false;
    idx_t d = digit_count(n);
    idx_t a = val2(n);
    idx_t b = val5(n);
    return (a == 0 || a >= d) && (b == 0 || b >= d);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for index 0; return 0 as safety */

    /* Generate terms until we have at least n entries. */
    while (seq_len < (size_t)n) {
        seq_t m = candidate++;
        /* Skip numbers coprime with 10. */
        if ((m & 1) && (m % 5))
            continue;

        if (is_k_morphic(m)) {
            ensure_capacity(seq_len + 1);
            seq[seq_len++] = m;
        }

        /* Safety: avoid infinite wrap‑around for very small
           seq_t types.  If candidate becomes 0, stop scanning. */
        if (candidate == 0)
            break;
    }

    if (n <= seq_len)
        return seq[(size_t)n - 1];
    /* If we ran out of values (e.g., overflow), return 0 as sentinel. */
    return 0;
}
