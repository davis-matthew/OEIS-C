/*
 * A341763  Numbers whose trajectory under iteration of sum of cubes of digits
 *          (map) produce a narcissistic number greater than nine.
 * https://oeis.org/A341763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   sum_cubes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A341763: search numbers whose sum‑of‑cubes‑of‑digits trajectory
   eventually reaches a 3‑digit narcissistic number > 9 (153, 370, 371, 407). */

#include <stdbool.h>
#include "../utils/sum_cubes.h"

static inline bool is_target(seq_t v)
{
    return v == (seq_t)153 ||
           v == (seq_t)370 ||
           v == (seq_t)371 ||
           v == (seq_t)407;
}

/* Test whether the trajectory of `start` reaches a target narcissistic number. */
static inline bool reaches_target(seq_t start)
{
    seq_t tortoise = start;
    seq_t hare     = start;
    while (1) {
        tortoise = sum_cubes(tortoise);
        if (is_target(tortoise))
            return true;
        hare = sum_cubes(hare);
        if (is_target(hare))
            return true;
        hare = sum_cubes(hare);
        if (is_target(hare))
            return true;
        if (tortoise == hare)
            return false;
    }
}

/* Return the n-th term of OEIS sequence A341763 (n ≥ 1). */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t cand  = 1;
    while (1) {
        if (reaches_target(cand)) {
            ++count;
            if (count == n)
                return cand;
        }
        ++cand;
    }
}
