/*
 * A342753  Concatenation of all 01-words, in the order induced by A001651;
 *          see Comments.
 * https://oeis.org/A342753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   bit_at_word, child_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A342753: concatenation of all 01‑words in the
    s‑induced order where s = A001651 (numbers not divisible by 3),
    t = A008585 (numbers divisible by 3).  For each word w(n):

        w(1)          = 0
        w(t(1))       = 1     (t(1)=3)
        if n∈s        w(n) = 0 w(s'(n))
        if n∈t        w(n) = 1 w(t'(n))

    where s'(n) = |{i≤n‑1 : i∈s}| and t'(n) = |{i≤n‑1 : i∈t}|.  The
    function returns the nth bit of the infinite binary string obtained
    by concatenating w(1), w(2), … .            */

#include <stdlib.h>
#include "../utils/bit_at_word.h"
#include "../utils/child_index.h"

/* Memoized array of word lengths: lens[k] = length of w(k).
   Indices start at 1; lens[0] is a sentinel with length 0.      */
static idx_t *lens  = NULL;        /* array of lengths            */
static size_t lens_cap = 0;        /* allocated capacity          */
static size_t lens_size = 0;       /* highest index computed      */

/* Ensure that lens[1..n] are computed.  The recursion in
   `child_index` guarantees that child indices are < n, so the
   lengths can be computed sequentially.                          */
static void ensure_len_up_to(idx_t n)
{
    if (n <= (idx_t)lens_size)
        return;

    if (lens == NULL) {
        lens_cap = 128;
        lens = (idx_t *)malloc(lens_cap * sizeof(idx_t));
        if (lens == NULL) exit(EXIT_FAILURE);
        lens[0] = 0;          /* sentinel for empty word      */
        lens_size = 0;
    }
    if ((idx_t)n >= lens_cap) {
        size_t new_cap = lens_cap;
        while ((idx_t)new_cap <= n)
            new_cap <<= 1;
        idx_t *tmp = (idx_t *)realloc(lens, new_cap * sizeof(idx_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        lens = tmp;
        lens_cap = new_cap;
    }

    for (size_t i = lens_size + 1; i <= (size_t)n; ++i) {
        if (i == 1) {
            lens[1] = 1;                     /* w(1) = "0"          */
        } else {
            idx_t child = child_index((idx_t)i);
            lens[i] = 1 + lens[child];
        }
    }
    lens_size = (size_t)n;
}

/* Main sequence function: returns the nth bit of the concatenated word string. */
seq_t a(idx_t n)
{
    idx_t cumulative = 0;   /* total length of words processed so far */
    idx_t word = 1;         /* current word index                    */

    for (;;) {
        ensure_len_up_to(word);

        if (cumulative + lens[word] >= n) {
            idx_t offset = n - cumulative; /* 1‑based offset within w(word) */
            return bit_at_word(word, offset);
        }
        cumulative += lens[word];
        ++word;
    }
}
