/*
 * A278938  Between two successive digits "2" there are 0, then 1, then 2,
 *          then 3, ... other digits.
 * https://oeis.org/A278938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 193, u16 n <= 1054, u32 n <= 1054, u64 n <= 1054,
 *            u128 n <= 1054, big any n
 * Helpers:   candidate_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A278938:
 *   a(1) = 0 and afterwards a(n) is the smallest unused integer that, when
 *   appended to the decimal string formed by a(1)..a(n‑1), keeps the
 *   positions of all '2' digits such that the gaps between successive '2's
 *   are 0,1,2,3,... respectively.
 *
 * The code builds the sequence incrementally, memoising already
 * computed values and the current state of the decimal string. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/candidate_valid.h"

/* ----------  helpers for sequence build  ---------- */

static seq_t *seq = NULL;          /* stored sequence values          */
static size_t seq_cap = 0;         /* capacity of seq array           */
static size_t seq_len = 0;         /* number of values stored         */

/* used[i] == 1  -> integer i already appears in the sequence   */
static uint8_t *used = NULL;
static size_t used_cap = 0;        /* number of used slots            */

static idx_t pos = 0;              /* next position in concatenated string          */
static idx_t last_two = (idx_t)-1; /* position of most recent '2', or sentinel     */
static idx_t twos   = 0;           /* number of '2's seen so far                  */

/* Ensure seq array has room for at least 'needed' elements.  */
static inline void ensure_seq_cap(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Ensure used array can address indices up to 'n' inclusively.  */
static inline void ensure_used_cap(idx_t n)
{
    if (n < (idx_t)used_cap) return;
    size_t new_cap = used_cap ? used_cap << 1 : 64;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    used = (uint8_t *)realloc(used, new_cap * sizeof(uint8_t));
    memset(used + used_cap, 0, new_cap - used_cap);  /* new slots 0 */
    used_cap = new_cap;
}

/* ----------  public sequence function  ---------- */

seq_t a(idx_t n)
{
    static bool inited = false;
    if (!inited) {
        inited = true;
        seq = NULL;        /* seq array will be allocated lazily */
        seq_cap = 0;
        seq_len = 0;
        used = NULL;
        used_cap = 0;
        pos = 0;
        last_two = (idx_t)-1;
        twos = 0;
    }

    while (seq_len < n) {
        ensure_seq_cap(seq_len + 1);

        idx_t cand = 0;
        for (;;) {
            ensure_used_cap(cand);
            if (used[cand]) { cand++; continue; }

            idx_t new_pos, new_last, new_twos;
            if (candidate_valid(cand, &pos, &last_two, &twos,
                                &new_pos, &new_last, &new_twos)) {
                seq[seq_len++] = (seq_t)cand;
                used[cand] = 1;
                pos = new_pos;
                last_two = new_last;
                twos = new_twos;
                break;
            }
            cand++;
        }
    }

    return seq[n-1];
}
