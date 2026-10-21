/*
 * A242437  Numbers not appearing in the sequence of integers, beginning with
 *          1, that can be formed by adding any digit of any previous term to
 *          that previous term.
 * https://oeis.org/A242437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   recompute_missing
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A242437: numbers not appearing in the sequence obtainable from 1 by
   repeatedly adding any decimal digit of the current term.
   The function finds the nth missing number by always recomputing the
   reachable set up to a limit that expands until at least n missing
   numbers are found. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/recompute_missing.h"

static uint8_t *global_seen   = NULL;
static size_t  global_seen_len = 0;
static idx_t  *global_missing = NULL;
static size_t  global_missing_len = 0;
static idx_t   current_limit    = 0;

static void ensure_missing(idx_t n) {
    while (global_missing_len < n) {
        idx_t new_limit = current_limit ? current_limit * 2 : 4;
        current_limit = new_limit;
        recompute_missing(current_limit, &global_seen, &global_seen_len,
                          &global_missing, &global_missing_len);
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for offset 1, but return 0 */
    ensure_missing(n);             /* guarantees global_missing_len >= n */
    return (seq_t)global_missing[n - 1];
}
