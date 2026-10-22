/*
 * A376521  Sorted positions of first appearances in the run-compression
 *          (A037201) of the first differences (A001223) of the prime numbers
 *          (A000040).
 * https://oeis.org/A376521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the run‑index of the n‑th new prime gap. */
/* It is obtained by scanning successive primes, recording each new gap
   that appears for the first time in the run‑compressed sequence of gaps. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for n = 0, return 0 */
        return (seq_t)0;

    /* ---------- static state ---------- */
    static bool initialized = false;
    static size_t pos_cap = 0;
    static size_t pos_len = 0;
    static seq_t *pos_arr = NULL;

    static size_t diff_cap = 0;
    static size_t diff_count = 0;
    static bool  *diff_seen = NULL;

    static idx_t last_prime = 0;
    static idx_t cand = 3;
    static idx_t last_diff = (idx_t)-1;   /* sentinel */
    static idx_t run_index = 0;           /* run‑compressed position */

    /* ---------- initialization ---------- */
    if (!initialized) {
        /* positions array */
        pos_cap = 16;
        pos_arr = (seq_t *)malloc(pos_cap * sizeof(seq_t));
        pos_len = 0;

        /* seen‑array for gaps */
        diff_cap = 64;
        diff_seen = (bool *)calloc(diff_cap, sizeof(bool));
        diff_count = 0;

        /* first prime */
        last_prime = 2;
        cand = 3;
        run_index = 0;
        initialized = true;
    }

    /* ---------- ensure we have at least n positions ---------- */
    while (pos_len < n) {
        /* find next prime */
        while (!is_prime(cand))
            cand += 2;                /* skip even numbers */

        /* new prime found */
        idx_t next_prime = cand;
        cand += 2;                     /* candidate for following prime */

        idx_t gap = next_prime - last_prime;

        /* run‑compression: process only when gap differs from previous */
        if (gap != last_diff) {
            ++run_index;                /* new element in compressed run */

            /* make sure diff_seen array can index 'gap' */
            if ((size_t)gap >= diff_cap) {
                size_t new_cap = gap * 2 + 1;
                bool *tmp = (bool *)realloc(diff_seen, new_cap * sizeof(bool));
                if (!tmp) { /* allocation failure: abort */
                    free(pos_arr);
                    free(diff_seen);
                    abort();
                }
                /* zero the newly added part */
                for (size_t i = diff_cap; i < new_cap; ++i)
                    tmp[i] = false;
                diff_seen = tmp;
                diff_cap = new_cap;
            }

            /* first time we see this gap value */
            if (!diff_seen[gap]) {
                diff_seen[gap] = true;
                ++diff_count;

                /* store its run‑index position */
                if (pos_len == pos_cap) {
                    size_t new_cap = pos_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(pos_arr, new_cap * sizeof(seq_t));
                    if (!tmp) { /* allocation failure: abort */
                        free(pos_arr);
                        free(diff_seen);
                        abort();
                    }
                    pos_arr = tmp;
                    pos_cap = new_cap;
                }
                pos_arr[pos_len++] = (seq_t)run_index;
            }

            last_diff = gap;
        }

        last_prime = next_prime;
    }

    return pos_arr[n - 1];
}
