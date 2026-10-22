/*
 * A353780  a(1) = 1; for n > 1, a(n) is the smallest positive number that has
 *          not yet appeared that is coprime to a(n-1), does not equal
 *          a(n-1)+1, and has no digit in common with a(n-1).
 * https://oeis.org/A353780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   recurrence-based sequence a(1) = 1; For n > 1, a(n) is the
 *            smallest positive integer that 1) has not yet appeared, 2) is
 *            coprime to a(n‑1), 3) is not a(n‑1)+1, 4) shares no decimal
 *            digit with a(n‑1)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* static storage for the sequence and a bitmap of used values */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0, seq_len = 0;
    static uint8_t *used = NULL;          /* bitmap: used[i] == 1  ⇒ i already used */
    static size_t used_cap = 0;           /* capacity of `used` in bytes */

    /* grow the sequence array to at least `n` elements */
    if (seq_len < (size_t)n) {
        if (seq_cap < (size_t)n) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 1;
            while (new_cap < (size_t)n) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            seq = tmp;
            seq_cap = new_cap;
        }
        /* initialise a(1) if the sequence is empty */
        if (seq_len == 0) {
            seq[0] = (seq_t)1;
            seq_len = 1;

            /* initialise used bitmap for value 1 */
            if (used_cap <= 1) {
                size_t new_cap = used_cap ? used_cap * 2 : 1;
                while (new_cap <= 1) new_cap *= 2;
                uint8_t *tmp = (uint8_t *)realloc(used, new_cap);
                if (!tmp) exit(1);
                if (tmp != used) memset(tmp + used_cap, 0, new_cap - used_cap);
                used = tmp;
                used_cap = new_cap;
            }
            used[1] = 1;
        }

        /* generate terms until we reach the requested index */
        while (seq_len < (size_t)n) {
            seq_t prev = seq[seq_len - 1];

            /* digit mask of previous term */
            unsigned prev_mask = 0;
            seq_t p = prev;
            if (p == 0) {
                prev_mask |= 1U;
            } else {
                while (p > 0) {
                    prev_mask |= 1U << (uint32_t)(p % 10);
                    p /= 10;
                }
            }

            seq_t candidate = 0;
            for (candidate = 1; ; ++candidate) {
                uint64_t idx = (uint64_t)candidate;
                if (idx < used_cap && used[idx]) continue;          /* already used */

                if (gcd(prev, candidate) != (seq_t)1) continue;    /* not coprime */

                if (candidate == prev + 1) continue;                /* consecutive */

                /* digit mask of candidate */
                unsigned cand_mask = 0;
                seq_t t = candidate;
                if (t == 0) {
                    cand_mask |= 1U;
                } else {
                    while (t > 0) {
                        cand_mask |= 1U << (uint32_t)(t % 10);
                        t /= 10;
                    }
                }
                if (prev_mask & cand_mask) continue;                /* digit clash */

                break;                                             /* candidate found */
            }

            /* append the new term */
            seq[seq_len++] = candidate;

            /* enlarge used bitmap if necessary and mark the candidate as used */
            uint64_t cand_idx = (uint64_t)candidate;
            if (cand_idx >= used_cap) {
                size_t new_cap = used_cap ? used_cap * 2 : 1;
                while (new_cap <= cand_idx) new_cap *= 2;
                uint8_t *tmp = (uint8_t *)realloc(used, new_cap);
                if (!tmp) exit(1);
                if (tmp != used)
                    memset(tmp + used_cap, 0, new_cap - used_cap);
                used = tmp;
                used_cap = new_cap;
            }
            used[cand_idx] = 1;
        }
    }

    return seq[n - 1];
}
