/*
 * A285498  1-limiting word of the morphism 0->11, 1-> 0010.
 * https://oeis.org/A285498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_word
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) is the nth bit of the 1‑limiting word of the morphism
   0 -> 11, 1 -> 0010.  The word is produced by starting from the
   word “11” (the first odd iteration).  Each subsequent odd word is
   obtained by applying the morphism twice.  The function builds the
   word until it contains at least n symbols and returns the (n‑1)‑th
   symbol (1‑based index). */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_word.h"

static unsigned char *odd_word = NULL;
static size_t odd_len = 0;

static void ensure_len(idx_t target)
{
    if (odd_len >= (size_t)target) return;
    if (!odd_word) {
        odd_word = (unsigned char *)malloc(2);
        odd_word[0] = 1;
        odd_word[1] = 1;
        odd_len = 2;
    }
    while (odd_len < (size_t)target) {
        unsigned char *tmp;
        size_t tmp_len;
        /* Even word from current odd word */
        next_word(odd_word, odd_len, &tmp, &tmp_len);
        free(odd_word);
        odd_word = tmp;
        odd_len = tmp_len;
        /* Next odd word from even word */
        next_word(odd_word, odd_len, &tmp, &tmp_len);
        free(odd_word);
        odd_word = tmp;
        odd_len = tmp_len;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_len(n);
    return (seq_t)odd_word[n - 1];
}
