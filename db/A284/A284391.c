/*
 * A284391  1-limiting word of the morphism 0 -> 1, 1 -> 001.
 * https://oeis.org/A284391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the 1‑limiting word of the morphism 0 → 1, 1 → 001
   (odd‑iteration limit word).  a(n) = the n‑th symbol of that word
   (1‑based).  The implementation expands the word repeatedly until
   it contains at least n symbols **and** the current iteration is odd.
   It keeps the generated word and its parity in static variables to
   allow multiple independent calls. */
#include <stdlib.h>

static seq_t *lim_word = NULL;   /* array of symbols, 0 or 1 */
static idx_t  lim_len  = 0;      /* current length of lim_word   */
static int    lim_parity = 0;    /* 0 = even iteration, 1 = odd iteration */

/* Expand the word by one morphism iteration.  Caller must update
   lim_len and lim_parity. */
static void expand(void)
{
    size_t new_cap = (size_t)lim_len * 3;        /* upper bound on new size */
    seq_t *new_word = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_word) abort();

    size_t pos = 0;
    for (size_t i = 0; i < (size_t)lim_len; ++i) {
        seq_t sym = lim_word[i];
        if (sym == 0) {
            new_word[pos++] = (seq_t)1;           /* 0 → 1 */
        } else {
            new_word[pos++] = (seq_t)0;           /* 1 → 0 0 1 */
            new_word[pos++] = (seq_t)0;
            new_word[pos++] = (seq_t)1;
        }
    }

    free(lim_word);
    lim_word = new_word;
    lim_len  = (idx_t)pos;
    lim_parity ^= 1;                             /* toggle parity */
}

/* Ensure that lim_word contains at least n symbols and that the
   current iteration count is odd. */
static void ensure_len(idx_t n)
{
    if (lim_word == NULL) {                     /* first‑time initialization */
        lim_word   = (seq_t *)malloc(sizeof(seq_t));
        if (!lim_word) abort();
        lim_word[0] = (seq_t)0;                 /* seed word = "0"       */
        lim_len    = 1;
        lim_parity = 0;                         /* iteration 0 is even   */
    }

    while (lim_len < n) {                       /* grow until enough length */
        expand();
    }

    if ((lim_parity & 1) == 0) {                /* need odd iteration */
        expand();
    }
}

seq_t a(idx_t n)
{
    /* Ensure that the internal word has at least n symbols
       and that we are at an odd iteration (1‑limiting word). */
    ensure_len(n);
    /* indices are 1‑based in OEIS; convert to 0‑based */
    return lim_word[(size_t)(n - 1)];
}
