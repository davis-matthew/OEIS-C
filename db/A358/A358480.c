/*
 * A358480  Number of 7's that appeared in the n-th step when constructing
 *          A030717.
 * https://oeis.org/A358480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   simulation of A030717. Step 1 is the initial list [1]. Step k >
 *            1 appends to the list how many times each distinct value already
 *            occurs in it, in ascending order of value. a(n) = number of 7's
 *            appended at step n
 * Range:     u8 n <= 56, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Validated: partial, pass, 128 terms through n = 128 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *keys = NULL, *cnts = NULL, *tmp = NULL, *res = NULL;
    static size_t size = 0, cap = 0, rcnt = 0, rcap = 0;

    if (keys == NULL) {
        cap = 64;
        keys = mem_new(seq_t, cap);
        cnts = mem_new(seq_t, cap);
        tmp = mem_new(seq_t, cap);
        rcap = 64;
        res = mem_new(seq_t, rcap);
        keys[0] = (seq_t)1;
        cnts[0] = (seq_t)1;
        size = 1;
        /* step 1: the initial list [1] contains no 7's */
        res[0] = (seq_t)0;
        rcnt = 1;
    }

    while (rcnt < (size_t)n) {
        size_t m = size;
        size_t i;
        seq_t sevens = 0;

        /* snapshot of frequencies in ascending key order */
        for (i = 0; i < m; i++) {
            tmp[i] = cnts[i];
            if (tmp[i] == (seq_t)7) sevens++;
        }

        if (rcnt >= rcap) {
            rcap *= 2;
            res = mem_resize(seq_t, res, rcap);
        }
        res[rcnt++] = sevens;

        /* add each frequency value to the list */
        for (i = 0; i < m; i++) {
            seq_t key = tmp[i];
            size_t lo = 0, hi = size;
            while (lo < hi) {
                size_t mid = lo + (hi - lo) / 2;
                if (keys[mid] < key) lo = mid + 1; else hi = mid;
            }
            if (lo < size && keys[lo] == key) {
                cnts[lo] += 1;
            } else {
                if (size + 1 > cap) {
                    cap *= 2;
                    keys = mem_resize(seq_t, keys, cap);
                    cnts = mem_resize(seq_t, cnts, cap);
                    tmp = mem_resize(seq_t, tmp, cap);
                }
                for (size_t j = size; j > lo; j--) {
                    keys[j] = keys[j - 1];
                    cnts[j] = cnts[j - 1];
                }
                keys[lo] = key;
                cnts[lo] = (seq_t)1;
                size++;
            }
        }
    }

    return res[n - 1];
}
