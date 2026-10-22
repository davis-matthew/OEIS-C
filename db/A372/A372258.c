/*
 * A372258  An infinite sequence over {0,1} with no palindromes of length >
 *          12.
 * https://oeis.org/A372258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + A014707(floor(n/2))) mod 2 (implemented via the
 *            morphic word 0→01,1→21,2→34,3→01,4→54,5→34, then coded modulo 2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static unsigned char *digits = NULL; /* raw digits 0‑5 of the morphic word */
static size_t len = 0;               /* current length of digits[] */

/*
 * Returns the n‑th term of OEIS A372258.  The function is thread‑safe
 * only for a single thread; it uses static memory for memoisation.
 */
seq_t a(idx_t n)
{
    /* initialise the seed word if not already done */
    if (digits == NULL) {
        digits = (unsigned char *)malloc(sizeof(unsigned char));
        if (digits == NULL) return 0;          /* allocation failure */
        digits[0] = 0;
        len = 1;
    }

    /* mapping table: each symbol 0‑5 maps to a pair of symbols */
    static const unsigned char map[6][2] = {
        {0, 1},  /* 0 -> 01 */
        {2, 1},  /* 1 -> 21 */
        {3, 4},  /* 2 -> 34 */
        {0, 1},  /* 3 -> 01 */
        {5, 4},  /* 4 -> 54 */
        {3, 4}   /* 5 -> 34 */
    };

    /* grow the word until it contains the requested position */
    while (len <= (size_t)n) {
        size_t old_len = len;
        size_t new_len = old_len << 1;          /* double the length */
        unsigned char *new_seq = (unsigned char *)malloc(new_len);
        if (new_seq == NULL) return 0;          /* allocation failure */

        /* expand the word using the morphism */
        for (size_t i = 0; i < old_len; ++i) {
            unsigned char idx = digits[i];      /* 0‑5 */
            new_seq[2 * i]     = map[idx][0];
            new_seq[2 * i + 1] = map[idx][1];
        }

        free(digits);
        digits = new_seq;
        len = new_len;
    }

    /* return the bit (digit mod 2) at position n */
    return (seq_t)(digits[n] & 1);
}
