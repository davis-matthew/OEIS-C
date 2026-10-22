/*
 * A332603  Working over the alphabet {1,2,3}, start with a(1) = 1; then
 *          a(n+1) is made by inserting a letter into a(n) at the rightmost
 *          possible position which makes a squarefree word (and the smallest
 *          letter if multiple letters are possible at that place).
 * https://oeis.org/A332603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   word_to_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A332603:  starting with a(1)=1; each a(n+1) is made by
   inserting a letter into a(n) at the rightmost possible position that
   keeps the word square‑free (choosing the smallest letter if several
   work).  The function returns a(n) as a decimal integer. */
    
#include <stdlib.h>
#include <stdint.h>
#include "../utils/word_to_value.h"

static const uint64_t BASE = 911382323ULL;

/* Storage for all computed words, 1‑based indices. */
static uint8_t **_words = NULL;
static size_t _words_cap = 0;
static size_t _max_computed = 0;

/* Pre‑computed powers of BASE modulo 2^64. */
static uint64_t *pow_tbl = NULL;
static size_t _pow_cap = 0;

/* Ensure that the power table can provide BASE^i for 0<=i<=needed. */
static void ensure_pow(size_t needed)
{
    if (_pow_cap <= needed)
    {
        size_t new_cap = needed + 1;
        uint64_t *tmp = (uint64_t*)realloc(pow_tbl, new_cap * sizeof(uint64_t));
        if (tmp == NULL) exit(1);
        pow_tbl = tmp;
        for (size_t i = _pow_cap; i < new_cap; ++i)
            pow_tbl[i] = (i==0) ? 1ULL : pow_tbl[i-1] * BASE;
        _pow_cap = new_cap;
    }
}

/* Ensure the words table can hold indices up to n. */
static void ensure_words_capacity(size_t n)
{
    if (_words_cap <= n)
    {
        size_t new_cap = n + 1;   /* indices 0..n */
        uint8_t **tmp = (uint8_t**)realloc(_words, new_cap * sizeof(uint8_t*));
        if (tmp == NULL) exit(1);
        for (size_t i = _words_cap; i < new_cap; ++i) tmp[i] = NULL;
        _words = tmp;
        _words_cap = new_cap;
    }
}

/* Test whether inserting 'letter' at position 'pos' into 'prev' (length L)
   would keep the resulting word square‑free.   Only squares that involve
   the new letter are examined. */
static int try_insert(const uint8_t *prev, size_t L, size_t pos,
                      uint8_t letter, uint64_t *h)
{
    size_t len = L + 1;
    /* Ensure we have enough powers for hashing. */
    ensure_pow(len);

    /* Build prefix hash of the candidate word. */
    h[0] = 0;
    for (size_t i = 0; i < len; ++i)
    {
        uint8_t v = (i == pos) ? letter
                     : (i < pos)   ? prev[i]
                                   : prev[i-1];
        h[i+1] = h[i] * BASE + v;
    }

    /* Check all squares that involve the new letter. */
    for (size_t l = 1; l <= len/2; ++l)
    {
        size_t i_start = (pos >= 2*l-1) ? pos-2*l+1 : 0;
        size_t i_end   = pos;
        if (i_end > len-2*l) i_end = len-2*l;
        for (size_t i = i_start; i <= i_end; ++i)
        {
            uint64_t left  = h[i+l]  - h[i]   * pow_tbl[l];
            uint64_t right = h[i+2*l] - h[i+l] * pow_tbl[l];
            if (left == right) return 0;   /* square found */
        }
    }
    return 1;   /* no square involving the new letter */
}

/* Build the next word in the sequence from 'prev' (length L). */
static uint8_t* build_next_word(const uint8_t *prev, size_t L)
{
    size_t len = L + 1;
    /* Temporary storage for prefix hashes (len+1 entries). */
    static uint64_t h_buf[20005];   /* maximum expected length ~10000 */

    for (size_t pos = L; ; --pos)
    {
        for (uint8_t letter = 1; letter <= 3; ++letter)
        {
            if (try_insert(prev, L, pos, letter, h_buf))
            {
                uint8_t *res = (uint8_t*)malloc(len);
                if (res == NULL) exit(1);
                /* Construct the new word. */
                for (size_t i = 0; i < pos; ++i) res[i] = prev[i];
                res[pos] = letter;
                for (size_t i = pos; i < L; ++i) res[i+1] = prev[i];
                return res;
            }
        }
        if (pos == 0) break;   /* should never happen */
    }
    return NULL;   /* safety */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined; harmless fallback */

    /* Initialisation: word 1 = "1". */
    if (_max_computed == 0)
    {
        ensure_words_capacity(1);
        uint8_t *w1 = (uint8_t*)malloc(1);
        if (w1 == NULL) exit(1);
        w1[0] = 1;
        _words[1] = w1;
        _max_computed = 1;
    }

    /* Extend the sequence up to the requested index n. */
    while (_max_computed < (size_t)n)
    {
        const uint8_t *prev = _words[_max_computed];
        size_t L = _max_computed;
        uint8_t *next = build_next_word(prev, L);
        if (next == NULL) exit(1);
        ++_max_computed;
        ensure_words_capacity(_max_computed);
        _words[_max_computed] = next;
    }

    /* Return the value of a(n). */
    return word_to_value(_words[n], (size_t)n);
}
