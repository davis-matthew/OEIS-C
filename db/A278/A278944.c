/*
 * A278944  Between two successive digits "9" there are 0, then 1, then 2,
 *          then 3, ... other digits.
 * https://oeis.org/A278944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Computation is by a greedy algorithm for A278944. The * sequence
 *            is built by always appending the smallest unused non‑negative *
 *            integer that does not violate the rule that the number of
 *            decimal digits * between successive 9 digits in the concatenated
 *            sequence equals * 0, 1, 2, 3, … in order. The...
 * Range:     u8 n <= 95, u16 n <= 1061, u32 n <= 1061, u64 n <= 1061,
 *            u128 n <= 1061, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Memoisation state ---------- */
static bool initialized = false;
static seq_t *seq = NULL;       /* array of generated terms, seq[0] dummy */
static size_t seq_cap = 0;
static size_t seq_len = 0;      /* number of elements stored (incl. dummy) */

static unsigned char *used = NULL; /* bitmask of numbers already used */
static size_t used_cap = 0;        /* capacity of the used array */

static bool have_seen_9 = false;
static uint32_t gap_counter = 0;    /* digits counted since last '9' */
static uint32_t next_gap_needed = 0;/* expected gap to the next '9' */

/* Ensure the used array can address values up to max_value */
static void ensure_used_capacity(uint64_t max_value) {
    if (max_value >= used_cap) {
        size_t new_cap = used_cap ? used_cap << 1 : 64;
        while (new_cap <= max_value) new_cap <<= 1;
        unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
        if (!tmp) exit(1); /* cannot continue */
        for (size_t i = used_cap; i < new_cap; ++i) tmp[i] = 0;
        used = tmp;
        used_cap = new_cap;
    }
}

/* Ensure the sequence array can hold at least new_len elements */
static void ensure_seq_capacity(size_t new_len) {
    if (new_len > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 4;
        while (new_cap < new_len) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        seq = tmp;
        seq_cap = new_cap;
    }
}

/* Try to append candidate v.  Return true if added, false otherwise. */
static bool try_add_candidate(uint64_t v) {
    uint32_t gap = gap_counter;
    uint32_t next_gap = next_gap_needed;
    bool seen = have_seen_9;
    bool first_9_in_candidate = false;

    /* Process the decimal digits of v (most significant first) */
    if (v == 0) {
        if (seen) gap++;                     /* a '0' after a 9 */
    } else {
        unsigned char digits[20];
        size_t len = 0;
        uint64_t t = v;
        while (t > 0) {
            digits[len++] = (unsigned char)(t % 10);
            t /= 10;
        }
        for (int i = (int)len - 1; i >= 0; --i) {
            unsigned char d = digits[i];
            if (d == 9) {
                if (!seen) {                /* first '9' of the whole sequence */
                    seen = true;
                    first_9_in_candidate = true;
                    next_gap = 0;
                    gap = 0;
                } else {                    /* subsequent '9' */
                    if (gap != next_gap) return false;     /* gap mismatch */
                    next_gap++;
                    gap = 0;
                }
            } else if (seen) {
                gap++;                         /* count digits after the last '9' */
            }
        }
    }

    if (seen && gap > next_gap) return false;   /* cannot still meet the required gap */

    /* Candidate is valid – commit state changes */
    have_seen_9 = seen;
    gap_counter = gap;
    next_gap_needed = next_gap;

    /* Mark as used and store in the sequence */
    ensure_used_capacity(v);
    used[v] = 1;
    ensure_seq_capacity(seq_len + 1);
    seq[seq_len++] = (seq_t)v;

    return true;
}

/* Public function – returns a(n) for any n >= 1. */
seq_t a(idx_t n) {
    if (!initialized) {
        initialized = true;

        /* initialise sequence array with dummy at index 0 */
        seq_cap = 4;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len = 1;            /* seq[0] is dummy */
        seq[0] = (seq_t)0;

        used_cap = 0;
        used = NULL;

        have_seen_9 = false;
        gap_counter = 0;
        next_gap_needed = 0;
    }

    while (seq_len <= (size_t)n) {
        /* find the smallest unused integer that is admissible */
        uint64_t cand = 0;
        while (true) {
            if (!(cand < used_cap && used[cand])) {
                if (try_add_candidate(cand))
                    break;   /* successfully added */
            }
            ++cand;
        }
    }

    return seq[(size_t)n];
}
