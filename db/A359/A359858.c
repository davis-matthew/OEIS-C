/*
 * A359858  a(0) = 0; for n > 0, a(n) is the smallest positive number not
 *          occurring earlier such that the ones' complement of the binary
 *          string of a(n-1) + a(n) does not appear in the binary string
 *          concatenation of a(0)..a(n-1).
 * https://oeis.org/A359858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A359858: a(0)=0; for n>0, a(n) is the smallest positive
 * number not occurring earlier such that the ones' complement of
 * the binary string of a(n‑1)+a(n) (after stripping leading zeros)
 * is not a substring of the binary concatenation of a(0)..a(n‑1).
 * Implementation uses a growing bit‑set for binary substrings up
 * to MAX_LEN bits.  Assumes that when the sequence reaches 10 000
 * terms, all occurring substrings are at most 23 bits long. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/bitlen_seq_t.h"

#define MAX_LEN 24            /* maximum length of substrings tracked  */

/* --- Storage for the sequence and the used numbers ------------- */
static seq_t *seq      = NULL;   /* sequence values                          */
static idx_t seq_size  = 0;      /* number of sequence elements known       */
static idx_t seq_cap   = 0;      /* allocated capacity for seq               */

/* --- List of numbers that have already appeared -------------- */
static seq_t *used_list = NULL;
static idx_t used_len   = 0;
static idx_t used_cap   = 0;

static void used_init(void)
{
    used_cap = 8;
    used_list = (seq_t *)malloc(used_cap * sizeof(seq_t));
    used_len = 0;
}

static bool used_find(seq_t v)
{
    for (idx_t i = 0; i < used_len; ++i)
        if (used_list[i] == v)
            return true;
    return false;
}

static void used_insert(seq_t v)
{
    if (used_find(v))
        return;
    if (used_len == used_cap) {
        used_cap <<= 1;
        used_list = (seq_t *)realloc(used_list, used_cap * sizeof(seq_t));
    }
    used_list[used_len++] = v;
}

/* --- Bit‑set of binary substrings -------------------------------- */
static uint64_t **patterns = NULL;

static void patterns_init(void)
{
    patterns = (uint64_t **)malloc((MAX_LEN + 1) * sizeof(uint64_t *));
    patterns[0] = NULL;
    for (int l = 1; l <= MAX_LEN; ++l) {
        size_t sz = (((size_t)1ULL << l) + 63) >> 6;       /* number of 64‑bit words */
        patterns[l] = (uint64_t *)calloc(sz, sizeof(uint64_t));
    }
}

static void set_pattern(int l, uint64_t pat)
{
    size_t idx = pat >> 6;
    uint64_t msk = 1ULL << (pat & 63);
    patterns[l][idx] |= msk;
}

static bool has_pattern(int l, uint64_t pat)
{
    size_t idx = pat >> 6;
    uint64_t msk = 1ULL << (pat & 63);
    return (patterns[l][idx] & msk) != 0;
}

/* --- Sliding window of the last MAX_LEN bits ------------------- */
static uint64_t window   = 0;
static idx_t   total_bits = 0;

static void process_bit(uint8_t bit)
{
    window = ((window << 1) | bit) & ((1ULL << MAX_LEN) - 1);
    ++total_bits;
    for (int l = 1; l <= MAX_LEN; ++l) {
        if (total_bits < (idx_t)l) break;
        uint64_t pat = window & ((1ULL << l) - 1);
        set_pattern(l, pat);
    }
}

static void addNumber(seq_t v)
{
    if (v == 0) {
        process_bit(0);
        return;
    }
    unsigned len = bitlen_seq_t(v);
    for (int i = (int)len - 1; i >= 0; --i) {
        uint8_t bit = (uint8_t)((v >> i) & 1ULL);
        process_bit(bit);
    }
}

/* --- Ensure sequence storage has enough capacity --------------- */
static void ensure_seq_cap(idx_t needed)
{
    if (needed <= seq_cap)
        return;
    idx_t new_cap = seq_cap ? seq_cap << 1 : 4;
    while (new_cap < needed)
        new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* --- Main sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        used_init();
        patterns_init();
        window = 0;
        total_bits = 0;
        ensure_seq_cap(1);
        seq[0] = 0;
        seq_size = 1;
        used_insert(0);
        addNumber(0);
        initialized = true;
    }

    if (n < seq_size)
        return seq[n];

    for (idx_t k = seq_size; k <= n; ++k) {
        seq_t prev = seq[seq_size - 1];
        seq_t cand = 1;
        while (true) {
            if (used_find(cand)) {
                ++cand;
                continue;
            }
            seq_t sum = prev + cand;
            unsigned len = bitlen_seq_t(sum);

            /* Build the binary pattern of 'sum' (MSB first) */
            uint64_t sum_pattern = 0;
            for (unsigned i = 0; i < len; ++i) {
                bool bit = ((sum >> (len - 1 - i)) & 1ULL) != 0;
                sum_pattern = (sum_pattern << 1) | (bit ? 1ULL : 0ULL);
            }

            /* Ones' complement of that pattern, limited to 'len' bits */
            uint64_t compl_pattern = (~sum_pattern) & ((1ULL << len) - 1);

            /* Determine effective length after stripping leading zeros */
            unsigned compl_len;
            if (compl_pattern == 0)
                compl_len = 1;
            else {
                compl_len = 0;
                uint64_t t = compl_pattern;
                while (t) {
                    ++compl_len;
                    t >>= 1;
                }
            }

            /* If complement length exceeds our tracked limit, reject candidate */
            if (compl_len > (unsigned)MAX_LEN) {
                ++cand;
                continue;
            }

            if (has_pattern((int)compl_len, compl_pattern)) {
                ++cand;
                continue;  /* pattern already seen */
            }
            break;          /* suitable candidate found */
        }

        /* Append the new term */
        ensure_seq_cap(seq_size + 1);
        seq[seq_size++] = cand;
        used_insert(cand);
        addNumber(cand);
    }
    return seq[n];
}
