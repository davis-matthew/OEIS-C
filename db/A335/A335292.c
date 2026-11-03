/*
 * A335292  To calculate a(n), iterate the function f(x) = x * ceiling(a(n-1)
 *          / 2) + 1 on the value x = 1 until the result has not appeared in
 *          the sequence previously. Take a(1) = 1.
 * https://oeis.org/A335292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined iteratively. To compute a(n) use a(1) = 1. For n
 *            > 1, let k = ceil(a(n-1)/2). Start with x = 1 and repeatedly set
 *            x = x*k + 1 until x has not appeared before in the sequence;
 *            then a(n) = x. The implementation keeps all terms in a dynamic
 *            array and performs a linear search to test...
 * Range:     u8 n <= 24, u16 n <= 223, u32 n <= 7264, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9778 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;   /* dynamic array of terms */
    static size_t len = 0;      /* number of terms stored */
    static size_t cap = 0;      /* allocated capacity */

    /* initialise with first term upon first call */
    if (len == 0) {
        grow_seq(&seq, &cap, 1);
        seq[0] = 1;
        len = 1;
    }

    /* if the requested index is already known, return immediately */
    if (n <= len)
        return seq[n - 1];

    /* grow the sequence until we reach the required term */
    while (len < n) {
        /* k = ceil(previous_term / 2) = (previous_term + 1) / 2 */
        seq_t prev = seq[len - 1];
        seq_t k = (prev + 1) / 2;

        seq_t x = 1;
        while (true) {
            /* linear search to see if x already appears */
            bool exists = false;
            for (size_t i = 0; i < len; ++i) {
                if (seq[i] == x) {
                    exists = true;
                    break;
                }
            }

            if (!exists) {
                /* new value, add to the sequence */
                grow_seq(&seq, &cap, len + 1);
                seq[len++] = x;
                break;
            }

            /* otherwise generate the next candidate */
            x = x * k + 1;
        }
    }

    return seq[n - 1];
}
