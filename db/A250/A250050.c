/*
 * A250050  Numbers n such that m = floor(n/5) is coprime to n and, if
 *          nonzero, m is also a term of the sequence.
 * https://oeis.org/A250050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 1732, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A250050 (RTC(b) for b=5):
   a(n) is the n‑th positive integer n such that
   m = floor(n/5) is coprime to n and, if m>0, m is also a term of the
   sequence.  The function generates terms incrementally, storing them
   in a static array and using binary search for membership checks. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd.h"

static seq_t *seq = NULL;          /* stored terms */
static size_t seq_len = 0;
static size_t seq_cap = 0;

static idx_t next_candidate = 1;   /* next integer to test */

/* Ensure the sequence array has room for one more element. */
static void ensure_seq_cap(void)
{
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 4;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq = tmp;
        seq_cap = new_cap;
    }
}

/* Binary search for a term in the sorted sequence array. */
static bool is_term(seq_t val)
{
    size_t low = 0, high = seq_len;
    while (low < high) {
        size_t mid = low + (high - low) / 2;
        if (seq[mid] < val)
            low = mid + 1;
        else if (val < seq[mid])
            high = mid;
        else
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n==0, return 0 */

    /* Generate terms until we have at least n of them. */
    while (seq_len < (size_t)n) {
        seq_t cand = (seq_t)next_candidate;
        seq_t m    = cand / (seq_t)5;          /* floor(cand/5) */

        if (gcd(cand, m) == (seq_t)1) {
            bool ok = (m == (seq_t)0) || is_term(m);
            if (ok) {
                ensure_seq_cap();
                seq[seq_len++] = cand;
            }
        }
        ++next_candidate;
    }
    return seq[(size_t)(n - 1)];
}
