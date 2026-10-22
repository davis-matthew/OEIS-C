/*
 * A348328  Records inventory sequence: Record the number of record terms seen
 *          thus far, then the number of zero terms, then number of 1's, then
 *          2's, etc until recording another zero term, at which point the
 *          count is repeated, as above.
 * https://oeis.org/A348328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   sequence defined by cycles as described in OEIS A348328
 * Range:     u8 n <= 4664, u16 n <= 9776, u32 n <= 9776, u64 n <= 9776,
 *            u128 n <= 9776, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Each cycle begins with the number of record terms seen so far, then
   the counts of 0,1,2,… until a zero count is produced (which terminates
   the cycle).  No closed‐form exists. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* static state */
static seq_t *seq = NULL;          /* sequence storage */
static size_t seq_len = 0, seq_cap = 0;

static uint64_t *freq = NULL;      /* frequency of each value seen */
static size_t freq_cap = 0;

static seq_t record_count = 0;     /* number of distinct record terms seen */
static seq_t current_max_record = 0;
static bool initialized = false;

/* ------------------------------------------------------------------ */
/* Ensure capacity for the sequence array                            */
static void ensure_seq_cap(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq = tmp;
    seq_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Ensure capacity for the frequency array                           */
static void ensure_freq_cap(size_t need)
{
    if (freq_cap >= need) return;
    size_t new_cap = freq_cap ? freq_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(freq, new_cap * sizeof(uint64_t));
    /* zero the new area */
    memset(tmp + freq_cap, 0, (new_cap - freq_cap) * sizeof(uint64_t));
    freq = tmp;
    freq_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Append a value to the sequence, updating frequencies and records   */
static void append_value(seq_t val)
{
    ensure_seq_cap(seq_len + 1);
    seq[seq_len++] = val;

    /* Update frequency map for the new value */
    idx_t idx = (idx_t)val;                /* value will be small in practice */
    if ((size_t)idx >= freq_cap) ensure_freq_cap((size_t)idx + 1);
    freq[idx]++;

    /* Update record statistics */
    if (val > current_max_record) {
        current_max_record = val;
        record_count++;
    }
}

/* ------------------------------------------------------------------ */
/* Generate terms up to and including index n (inclusive)            */
static void generate_up_to(idx_t n)
{
    while (seq_len <= (size_t)n) {
        /* Start a new cycle: record count term */
        append_value(record_count);

        /* Then output the counts of 0,1,2,… until a zero count is hit. */
        for (idx_t v = 0; ; ++v) {
            uint64_t cnt = (v < freq_cap) ? freq[v] : 0;
            seq_t val = (seq_t)cnt;
            append_value(val);
            if (cnt == 0) break;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public interface                                                    */
seq_t a(idx_t n)
{
    if (!initialized) {
        /* Initial term a(0) = 0 */
        seq_cap = 16;
        seq = (seq_t *)calloc(seq_cap, sizeof(seq_t));
        seq[0] = (seq_t)0;
        seq_len = 1;

        freq_cap = 16;
        freq = (uint64_t *)calloc(freq_cap, sizeof(uint64_t));
        freq[0] = 1;                     /* 0 has been seen once */

        record_count = (seq_t)1;          /* 0 is a record */
        current_max_record = (seq_t)0;
        initialized = true;
    }

    generate_up_to(n);
    return seq[n];
}
