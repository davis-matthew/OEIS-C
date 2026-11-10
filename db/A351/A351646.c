/*
 * A351646  a(n) = A097465(n) - n.
 * https://oeis.org/A351646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = A097465(n) - n, where A097465 is computed by greedy
 *            search: A097465(1) = 1; A097465(n) = smallest unused positive
 *            integer coprime to A097465(n-1) and not equal to A097465(n-1) +-
 *            1
 * Range:     i8 n <= 76, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 76 (2026-11-10)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

static seq_t A097465_a(idx_t n) {
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;
    static seq_t last = 0;
    static uint8_t *used = NULL;
    static size_t used_cap = 0;

    if (seq == NULL) {
        seq_cap = 16;
        seq = mem_new(seq_t, seq_cap);
        seq[0] = (seq_t)1;
        seq_len = 1;
        last = (seq_t)1;
        used_cap = 65536;
        used = (uint8_t*)calloc(used_cap, sizeof(uint8_t));
        used[1] = 1;
    }

    if (n <= seq_len) return seq[n-1];

    for (idx_t k = seq_len + 1; k <= n; ++k) {
        seq_t candidate = 1;
        for (;;) {
            idx_t idx = (idx_t)candidate;
            if (idx >= used_cap) {
                size_t new_cap = used_cap * 2;
                while (idx >= new_cap)
                    new_cap *= 2;
                uint8_t *tmp = (uint8_t*)realloc(used, new_cap * sizeof(uint8_t));
                memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(uint8_t));
                used = tmp;
                used_cap = new_cap;
            }
            if (!used[idx] &&
                candidate != last + (seq_t)1 &&
                candidate != last - (seq_t)1 &&
                gcd(candidate, last) == 1) {
                break;
            }
            ++candidate;
        }
        used[(idx_t)candidate] = 1;

        if (seq_len >= seq_cap) {
            size_t new_cap = seq_cap * 2;
            seq_t *tmp = mem_resize(seq_t, seq, new_cap);
            seq = tmp;
            seq_cap = new_cap;
        }
        seq[seq_len++] = candidate;
        last = candidate;
    }
    return seq[n-1];
}

seq_t a(idx_t n) {
    return A097465_a(n) - (seq_t)n;
}
