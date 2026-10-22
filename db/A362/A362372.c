/*
 * A362372  Inventory of powers. Initialize the sequence with '1'. Then record
 *          the number of powers of 1 thus far, then do the same for powers of
 *          2 (2, 4, 8, ...), powers of 3, etc. When the count is zero, do not
 *          record a zero; rather start the inventory again with the powers of
 *          1.
 * https://oeis.org/A362372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 1017, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A362372 – Inventory of powers.
   The sequence is defined by:
      a(0) = 1
      For k ≥ 1:
         let b be the smallest positive integer such that the count of
         numbers already in a[0..k-1] which are powers of b is positive;
         then a(k) is that count.
         Afterwards b is set to the next integer; if the count is zero,
         b resets to 1 and the process repeats.  The sequence is
         generated lazily and memoised.  No external libraries are
         required beyond the provided seq_* utilities. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Forward declarations for helper functions */
static void seq_ensure(size_t need);
static void freq_ensure(size_t need);
static size_t freq_index(seq_t key);
static seq_t freq_get(seq_t key);
static void freq_inc(seq_t key);
static seq_t count_powers_of_base(idx_t base);

/* Sequence storage and bookkeeping */
static seq_t *seq  = NULL;        /* main sequence array */
static size_t seq_len = 0;
static size_t seq_cap = 0;

static seq_t *freq_keys = NULL;  /* frequency table keys */
static seq_t *freq_cnt  = NULL;  /* corresponding counts */
static size_t freq_len = 0;
static size_t freq_cap = 0;

static idx_t cur_base = 1;        /* current inventory base */
static seq_t max_val = 0;         /* maximum term seen so far */

/* Ensure the main sequence array has space for `need` elements */
static void seq_ensure(size_t need) {
    if (need > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 64;
        if (new_cap < need) new_cap = need;
        seq = seq_realloc(seq, seq_cap, new_cap);
        seq_cap = new_cap;
    }
}

/* Ensure the frequency table arrays have space for `need` entries */
static void freq_ensure(size_t need) {
    if (need > freq_cap) {
        size_t new_cap = freq_cap ? freq_cap << 1 : 64;
        if (new_cap < need) new_cap = need;
        freq_keys = seq_realloc(freq_keys, freq_cap, new_cap);
        freq_cnt  = seq_realloc(freq_cnt,  freq_cap, new_cap);
        freq_cap = new_cap;
    }
}

/* Linear search for a key in the frequency table.
   Returns the index or freq_len if not found. */
static size_t freq_index(seq_t key) {
    for (size_t i = 0; i < freq_len; ++i)
        if (freq_keys[i] == key)
            return i;
    return freq_len;
}

/* Get the current count for a key (0 if absent). */
static seq_t freq_get(seq_t key) {
    size_t i = freq_index(key);
    return (i == freq_len) ? (seq_t)0 : freq_cnt[i];
}

/* Increment the count for a key, adding it if necessary. */
static void freq_inc(seq_t key) {
    size_t i = freq_index(key);
    if (i == freq_len) {
        freq_ensure(freq_len + 1);
        freq_keys[freq_len] = key;
        freq_cnt [freq_len] = (seq_t)1;
        ++freq_len;
    } else {
        ++freq_cnt[i];
    }
}

/* Count how many powers of `base` are already in the sequence. */
static seq_t count_powers_of_base(idx_t base) {
    seq_t count = 0;
    if (base == 1) {
        count = freq_get((seq_t)1);
    } else {
        seq_t b = (seq_t)base;
        for (seq_t p = b; p <= max_val; p *= b) {
            count += freq_get(p);
            if (p > max_val / b) break;   /* avoid overflow */
        }
    }
    return count;
}

/* Public accessor for the sequence. */
seq_t a(idx_t n) {
    /* initialise on first call */
    if (!seq) {
        seq_ensure(64);
        seq_len = 0;
        seq_cap = 64;

        freq_ensure(16);
        freq_len = 0;
        freq_cap = 16;

        /* first term is 1 */
        seq[0] = (seq_t)1;
        seq_len = 1;
        freq_inc((seq_t)1);
        max_val = (seq_t)1;
        cur_base = 1;
    }

    /* generate entries until we have at least n+1 terms (0‑based) */
    while (seq_len <= (size_t)n) {
        seq_t term;
        /* find next positive count */
        while (1) {
            seq_t count = count_powers_of_base(cur_base);
            if (count > 0) {
                term = count;
                ++cur_base;
                break;
            }
            cur_base = 1;   /* reset inventory if count is zero */
        }

        seq_ensure(seq_len + 1);
        seq[seq_len++] = term;
        freq_inc(term);
        if (term > max_val) max_val = term;
    }

    return seq[(size_t)n];
}
