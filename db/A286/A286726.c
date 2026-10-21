/*
 * A286726  {0->10, 1->01}-transform of the Pell word, A171588.
 * https://oeis.org/A286726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th bit of the word obtained from the Pell word
 *            A171588 by the morphism 0→10, 1→01
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static unsigned char *pell_word = NULL;   /* bits of the Pell word */
static size_t           pell_len  = 0;    /* current length of pell_word */

/* Build the Pell word until it contains at least indices 0..needed.  */
static void ensure_pell_len(size_t needed)
{
    if (pell_len > needed)
        return;

    /* Initialisation: start with the single symbol 0. */
    if (pell_word == NULL) {
        pell_word = (unsigned char *)malloc(1);
        if (!pell_word) exit(1);
        pell_word[0] = 0;
        pell_len = 1;
    }

    /* Morphism: 0 → 0 0 1 , 1 → 0 */
    while (pell_len <= needed) {
        size_t old_len = pell_len;
        /* 0 expands to 3 bits, 1 to 1 bit → at most 3*old_len bits. */
        unsigned char *new_word = (unsigned char *)malloc(old_len * 3 + 1);
        if (!new_word) exit(1);

        size_t pos = 0;
        for (size_t i = 0; i < old_len; ++i) {
            if (pell_word[i] == 0) {
                new_word[pos++] = 0;
                new_word[pos++] = 0;
                new_word[pos++] = 1;
            } else {                /* 1 → 0 */
                new_word[pos++] = 0;
            }
        }

        free(pell_word);
        pell_word = new_word;
        pell_len  = pos;
    }
}

seq_t a(idx_t n)
{
    /* index in the Pell word (0‑based) */
    size_t k = (size_t)((n - 1) / 2);
    ensure_pell_len(k);

    unsigned char p = pell_word[k];     /* 0 or 1 */

    /* Transform 0→10, 1→01 :
       odd index (first bit of the pair) gives complement of p,
       even index (second bit) gives p itself. */
    seq_t result = ((n & 1) ? (seq_t)(!p) : (seq_t)p);
    return result;
}
