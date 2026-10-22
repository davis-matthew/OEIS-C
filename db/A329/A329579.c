/*
 * A329579  For every n >= 0, exactly nine sums are prime among a(n+i) +
 *          a(n+j), 0 <= i < j < 7; lexicographically earliest such sequence
 *          of distinct nonnegative numbers.
 * https://oeis.org/A329579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Implements the lexicographically earliest permutation of the
   non‑negative integers in which every block of 7 consecutive terms
   contains exactly nine prime sums among the 21 pairwise sums.
   The sequence is built greedily: each new term is the smallest unused
   integer that, together with the six immediately preceding terms,
   yields exactly nine primes in the resulting window.
   All previously computed terms are cached so that any a(n) can be
   returned in O(1) after the necessary terms have been generated.
   The function is fully memoised and works with any build that
   defines seq_t as an unsigned integer type of 8‑, 16‑, 32‑, 64‑ or
   128‑bit width or as an arbitrary‑precision integer class, and
   idx_t as uint64_t.
*/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/isPrimeGeneric.h"

/* Cache for already generated terms */
static seq_t *SEQ = NULL;
static size_t SEQ_CAP = 0;
static size_t SEQ_LEN = 0;

/* List of values that have already been used (strictly increasing order
   is not required, a linear search is simplest). */
static seq_t *USED = NULL;
static size_t USED_CAP = 0;
static size_t USED_LEN = 0;

/* Ensure the SEQ array can hold at least 'need' elements */
static void ensure_seq_cap(size_t need)
{
    if (need <= SEQ_CAP) return;
    size_t new_cap = SEQ_CAP ? SEQ_CAP * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *p = (seq_t *)realloc(SEQ, new_cap * sizeof(seq_t));
    if (!p) exit(EXIT_FAILURE);
    SEQ = p;
    SEQ_CAP = new_cap;
}

/* Ensure the USED array can hold at least 'need' elements */
static void ensure_used_cap(size_t need)
{
    if (need <= USED_CAP) return;
    size_t new_cap = USED_CAP ? USED_CAP * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *p = (seq_t *)realloc(USED, new_cap * sizeof(seq_t));
    if (!p) exit(EXIT_FAILURE);
    USED = p;
    USED_CAP = new_cap;
}

/* Linear search for membership in USED */
static bool used_contains(seq_t val)
{
    for (size_t i = 0; i < USED_LEN; ++i)
        if (USED[i] == val) return true;
    return false;
}

/* Count how many of the 21 pairwise sums of the 7 numbers in win
   are prime. */
static int prime_sum_count(const seq_t *win)
{
    int pc = 0;
    for (int i = 0; i < 7; ++i)
        for (int j = i + 1; j < 7; ++j) {
            seq_t s = win[i] + win[j];
            if (isPrimeGeneric(s)) ++pc;
        }
    return pc;
}

/* Public function: returns the n-th term of the sequence. */
seq_t a(idx_t n)
{
    size_t target = (size_t)n;
    if (SEQ_LEN <= target) {
        /* Grow the sequence cache. */
        ensure_seq_cap(target + 1);
        while (SEQ_LEN <= target) {
            seq_t cand = 0;
            /* Find the smallest unused candidate that satisfies the
               window constraint once it becomes fully defined. */
            while (true) {
                if (!used_contains(cand)) {
                    if (SEQ_LEN < 6) {
                        /* No window complete yet – accept the candidate. */
                        break;
                    } else {
                        /* Build the window of six previous terms plus cand. */
                        seq_t win[7];
                        for (int i = 0; i < 6; ++i)
                            win[i] = SEQ[SEQ_LEN - 6 + i];
                        win[6] = cand;
                        if (prime_sum_count(win) == 9) break;
                    }
                }
                ++cand;
            }
            /* Accept the candidate. */
            seq_t val = cand;
            SEQ[SEQ_LEN] = val;
            /* Record that this value has been used. */
            ensure_used_cap(USED_LEN + 1);
            USED[USED_LEN++] = val;
            ++SEQ_LEN;
        }
    }
    return SEQ[target];
}
