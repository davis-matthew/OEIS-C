/*
 * A335481  Numbers k such that the k-th composition in standard order
 *          (A066099) matches the pattern (2,1,3).
 * https://oeis.org/A335481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A335481: numbers k such that the k‑th composition in standard order
   (A066099) matches the pattern (2,1,3). This implementation enumerates
   compositions in graded reverse‑lexicographic order and returns the
   n‑th such k. */

/* Required types (supplied by the build system):
   - idx_t  : unsigned integer type for indices (e.g., uint64_t)
   - seq_t  : unsigned or arbitrary‑precision type for sequence values
*/
#include <stdint.h>
#include <stdbool.h>

/* ---------- Helper data ---------- */
static uint64_t parts[64];          /* holds a composition being built   */
static idx_t  target_n;            /* value of n for the current call  */
static seq_t  result_k;            /* k corresponding to the n‑th match */
static bool   found;               /* true once the n‑th match is found*/
static idx_t  matches;             /* number of matches found so far   */
static seq_t  current_k;           /* global composition counter       */

/* ---------- Pattern matching ---------- */
static bool matches_pattern(const uint64_t *p, int len)
{
    /* return true if there exist indices i < j < k with
       p[j] < p[i] < p[k] (pattern (2,1,3) is present) */
    for (int i = 0; i < len; ++i) {
        for (int j = i + 1; j < len; ++j) {
            if (p[j] < p[i]) {
                for (int k = j + 1; k < len; ++k) {
                    if (p[i] < p[k]) return true;
                }
            }
        }
    }
    return false;
}

/* ---------- Recursive generation of compositions ----------
   Enumerates all compositions of 'rem' in reverse‑lexicographic order.
   'depth' is the current position in the composition. */
static void gen(uint64_t rem, int depth)
{
    for (uint64_t fp = rem; ; --fp) {
        parts[depth] = fp;
        uint64_t new_rem = rem - fp;
        if (new_rem == 0) {                     /* complete composition */
            current_k = current_k + 1;           /* increment global index */
            if (matches_pattern(parts, depth + 1)) {
                matches = matches + 1;
                if (matches == target_n) {
                    result_k = current_k;
                    found = true;
                    return;
                }
            }
        } else {                                 /* recurse on remaining sum */
            gen(new_rem, depth + 1);
            if (found) return;
        }
        if (fp == 1) break;                      /* avoid unsigned underflow */
    }
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    /* initialise local state for this call */
    target_n = n;
    found = false;
    matches = 0;
    current_k = 0;

    uint64_t sum = 1;          /* start from total sum 1 */
    while (!found) {
        gen(sum, 0);           /* generate all compositions of this sum */
        if (found) break;
        ++sum;
    }

    return result_k;
}
