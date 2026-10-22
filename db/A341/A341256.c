/*
 * A341256  Concatenation of all 01-words in the order induced by A004526; see
 *          Comments.
 * https://oeis.org/A341256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Generate the 01‑words w(n) in the order defined by A004526,
   concatenate them, and return the nth bit (0‑based) as a seq_t value. */

#include <stdint.h>
#include <stddef.h>

/* Max number of words needed for n up to about 10000. */
#define MAX_WORDS   20000
/* Max suffixes per prefix (same order of magnitude). */
#define MAX_SUFFIX  MAX_WORDS

/* Store each word as (len, val). len <= 16, val fits in 16 bits. */
static uint16_t word_len[MAX_WORDS + 1];
static uint16_t word_val[MAX_WORDS + 1];
static size_t   word_cnt = 0;  /* number of words generated */

/* Suffix sets for prefix 0 and 1.  Each suffix is (len, val). */
static uint16_t suffix_len[2][MAX_SUFFIX];
static uint16_t suffix_val[2][MAX_SUFFIX];
static size_t   suffix_cnt[2] = {0, 0};

/* Total length of the concatenated stream so far (idx_t). */
static idx_t total_len = 0;

/* Helper: check if a word (len, val) is present in suffix set of given prefix. */
static int suffix_has(int prefix, uint16_t len, uint16_t val)
{
    for (size_t i = 0; i < suffix_cnt[prefix]; ++i)
        if (suffix_len[prefix][i] == len && suffix_val[prefix][i] == val)
            return 1;
    return 0;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Initialise first two words. */
    if (word_cnt == 0) {
        /* w(1) = "0" */
        word_len[1] = 1;
        word_val[1] = 0;
        word_cnt = 1;
        /* suffix of w(1) after prefix: empty suffix of length 0 */
        suffix_len[0][0] = 0;
        suffix_val[0][0] = 0;
        suffix_cnt[0] = 1;
        total_len = 1;

        /* w(2) = "1" */
        word_len[2] = 1;
        word_val[2] = 1;
        word_cnt = 2;
        suffix_len[1][0] = 0;
        suffix_val[1][0] = 0;
        suffix_cnt[1] = 1;
        total_len = 2;
    }

    /* Generate words until we have at least n bits. */
    while (total_len < n) {
        /* Find smallest k not in suffix set of prefix 0. */
        size_t k0 = 0;
        for (size_t k = 1; k <= word_cnt; ++k)
            if (!suffix_has(0, word_len[k], word_val[k])) { k0 = k; break; }

        /* Find smallest k not in suffix set of prefix 1. */
        size_t k1 = 0;
        for (size_t k = 1; k <= word_cnt; ++k)
            if (!suffix_has(1, word_len[k], word_val[k])) { k1 = k; break; }

        /* Choose prefix with smaller k; on tie choose 0. */
        int prefix = (k0 <= k1) ? 0 : 1;
        size_t k = (prefix == 0) ? k0 : k1;

        /* Build new word: (prefix) concatenated with w(k). */
        uint16_t len_suf = word_len[k];
        uint16_t val_suf = word_val[k];
        uint16_t new_len = len_suf + 1;
        uint16_t new_val = ((uint16_t)prefix << len_suf) | val_suf;

        /* Store new word. */
        ++word_cnt;
        word_len[word_cnt] = new_len;
        word_val[word_cnt] = new_val;

        /* Add its suffix to the corresponding suffix set. */
        suffix_len[prefix][suffix_cnt[prefix]] = len_suf;
        suffix_val[prefix][suffix_cnt[prefix]] = val_suf;
        ++suffix_cnt[prefix];

        /* Update total length. */
        total_len += (idx_t)new_len;
    }

    /* Locate the word that contains position n. */
    idx_t remaining = n;
    size_t i = 0;
    while (i < word_cnt) {
        ++i;
        if (remaining <= (idx_t)word_len[i])
            break;
        remaining -= (idx_t)word_len[i];
    }

    /* Now i is the index of the word, and remaining is the 1‑based
       position within that word.  Extract the bit. */
    uint16_t len = word_len[i];
    uint16_t val = word_val[i];
    uint16_t pos = (uint16_t)(remaining - 1);          /* zero‑based from left */
    uint16_t shift = len - 1 - pos;                   /* right‑shift amount */
    uint16_t bit   = (val >> shift) & 1u;

    return (seq_t)bit;
}
