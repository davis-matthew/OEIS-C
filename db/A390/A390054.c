/*
 * A390054  a(1) = 1; for n > 1, a(n) is the least number not already in the
 *          sequence that is a multiple of the largest digit of a(n-1).
 * https://oeis.org/A390054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1; for n > 1, a(n) is the least number not already in the
 *            sequence that is a multiple of the largest digit of a(n-1). The
 *            algorithm keeps a hash table of used values and, for each
 *            possible largest digit (1..9), remembers the next multiple of
 *            that digit that has not yet been examined
 * Range:     u8 n <= 154, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static const size_t USED_TABLE_SIZE = 1u << 15;     /* 32768 - power of two */
static seq_t *used_table = NULL;
static size_t used_mask = 0;

static inline size_t hash_seq_t(seq_t x)
{
    /* Simple mixing: XOR low and high 64‑bit parts (works for 8/16/32/64/128). */
    size_t low = (size_t)(x);
    size_t high = 0;
    if (sizeof(seq_t) > 8) {
        high = (size_t)(x >> 64);
    }
    return low ^ high;
}

static void init_used_table(void)
{
    if (used_table != NULL) return;
    used_table = (seq_t *)calloc(USED_TABLE_SIZE, sizeof(seq_t));
    used_mask = USED_TABLE_SIZE - 1;
}

static inline bool used_contains(seq_t val)
{
    if (!used_table) return false;
    size_t idx = hash_seq_t(val) & used_mask;
    while (used_table[idx] != 0) {
        if (used_table[idx] == val) return true;
        idx = (idx + 1) & used_mask;
    }
    return false;
}

static inline void used_insert(seq_t val)
{
    init_used_table();
    size_t idx = hash_seq_t(val) & used_mask;
    while (used_table[idx] != 0) {
        idx = (idx + 1) & used_mask;
    }
    used_table[idx] = val;
}

static unsigned largest_digit(seq_t x)
{
    unsigned md = 0;
    while (x != 0) {
        unsigned d = (unsigned)(x % (seq_t)10);
        if (d > md) md = d;
        if (md == 9) break;
        x = x / (seq_t)10;
    }
    return md;
}

static seq_t *seq = NULL;          /* dynamically grown array of computed terms */
static idx_t seq_len = 0;          /* number of terms currently stored */
static size_t seq_cap = 0;         /* current capacity of seq array */

static void grow_seq(idx_t needed)
{
    if (needed <= (idx_t)seq_cap) return;
    size_t new_cap = (seq_cap < 16) ? 16 : seq_cap;
    while ((idx_t)new_cap < needed) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
    if (seq_len == 0 && seq_cap > 0) {
        /* Mark first slot as unused (but we never use 0 as a value). */
        seq[0] = 0;
    }
}

/* Static table holding the next multiple to try for each digit 1..9. */
static seq_t next_mult[10] = {0};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 so this is undefined */

    if (n <= seq_len) return seq[n-1];

    /* initialise if first call */
    if (seq == NULL) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        /* index 0 unused, but harmless */
        seq_len = 0;
        for (int d = 1; d <= 9; ++d) next_mult[d] = (seq_t)d;
    }

    while (seq_len < n) {
        seq_t prev = seq_len ? seq[seq_len-1] : (seq_t)1;
        unsigned d = largest_digit(prev);      /* d in 1..9 */

        seq_t cand = next_mult[d];
        while (used_contains(cand)) {
            cand += (seq_t)d;
        }

        /* store result */
        if (seq_len == seq_cap) grow_seq(seq_len + 1);
        seq[seq_len] = cand;
        used_insert(cand);
        next_mult[d] = cand + (seq_t)d;
        seq_len++;
    }

    return seq[n-1];
}
