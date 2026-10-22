/*
 * A351001  a(0) = 0, a(1) = 1; for n > 1, a(n) is the smallest positive
 *          number which has not appeared which has a common factor with
 *          a(n-2) + a(n-1) but does not equal a(n-2) + a(n-1).
 * https://oeis.org/A351001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, for n>1, * a(n)=smallest positive integer not
 *            yet used that is not equal to * a(n-2)+a(n-1) and satisfies
 *            gcd(a(n-2)+a(n-1), a(n))>1
 * Range:     u8 n <= 58, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"

/* ------------------- state ------------------- */
static seq_t *seq = NULL;          /* permutation values */
static size_t seq_cap = 0;         /* capacity of seq array   */
static idx_t  seq_len = 0;         /* number of terms stored  */

static uint8_t *used = NULL;       /* used[0..] 0/1 flags */
static size_t used_cap = 0;        /* capacity of used array   */

/* ------------------- helpers ------------------- */
static void used_set(uint64_t v)
{
    if (v >= used_cap) {
        size_t new_cap = used_cap > 0 ? used_cap : 1;
        while (v >= new_cap) new_cap <<= 1;
        used = (uint8_t*)realloc(used, new_cap * sizeof(uint8_t));
        memset(used + used_cap, 0, (new_cap - used_cap) * sizeof(uint8_t));
        used_cap = new_cap;
    }
    used[(size_t)v] = 1;
}

static bool used_get(uint64_t v)
{
    if (v >= used_cap) return false;
    return used[(size_t)v] != 0;
}

static void ensure_sequence_capacity(idx_t n)
{
    if (seq_cap == 0) {
        seq_cap = 8;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    }
    while ((size_t)n >= seq_cap) {
        seq_cap <<= 1;
        seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
    }

    if (seq_len == 0) {          /* a(0) */
        seq[0] = (seq_t)0;
        used_set(0);
        seq_len = 1;
    }
    if (seq_len == 1) {          /* a(1) */
        seq[1] = (seq_t)1;
        used_set(1);
        seq_len = 2;
    }
    if (seq_len == 2) {          /* a(2) */
        seq[2] = (seq_t)2;
        used_set(2);
        seq_len = 3;
    }
}

/* find the next unused number satisfying the recurrence */
static seq_t find_next(seq_t sum)
{
    uint64_t cand_val = 1;
    while (true) {
        seq_t cand_seq = (seq_t)cand_val;
        if (cand_seq == sum) {            /* cannot equal the sum */
            cand_val++;
            continue;
        }
        if (used_get(cand_val)) {          /* already used */
            cand_val++;
            continue;
        }
        if (gcd(sum, cand_seq) > (seq_t)1)  /* share a factor */
            return cand_seq;
        cand_val++;
    }
}

/* ------------------- main function ------------------- */
seq_t a(idx_t n)
{
    ensure_sequence_capacity(n);

    /* compute new terms up to n */
    while (seq_len <= n) {
        seq_t val;
        if (seq_len == 0)          val = (seq_t)0;
        else if (seq_len == 1)     val = (seq_t)1;
        else if (seq_len == 2)     val = (seq_t)2;
        else {
            seq_t sum = seq[seq_len - 2] + seq[seq_len - 1];
            val = find_next(sum);
        }
        seq[seq_len] = val;
        used_set((uint64_t)val);
        ++seq_len;
    }

    return seq[n];
}
