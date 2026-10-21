/*
 * A287801  {0->100, 1->001}-transform of the infinite Fibonacci word A003849.
 * https://oeis.org/A287801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th bit of the {0→100, 1→001}‐transform of the
 *            infinite Fibonacci word. Implementation: generate the Fibonacci
 *            word by repeatedly applying the morphism 0→01, 1→0 until the
 *            required position is reached, then translate the bit (0→100,
 *            1→001)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Dynamic storage for the Fibonacci word as a sequence of bits (0 or 1). */
static unsigned char *fib_word = NULL;
static size_t fib_len = 0;

/* Keep fib_word at least need+1 bits long (0‑based index). */
static void ensure_fib_word(size_t need)
{
    if (fib_len > need)            /* already reachable */
        return;

    /* Initialise with the single symbol '0'. */
    if (!fib_word) {
        fib_word = (unsigned char *)malloc(1);
        fib_word[0] = 0;
        fib_len   = 1;
    }

    /* Expand until we reach the needed index. */
    while (fib_len <= need) {
        size_t new_len = 0;
        unsigned char *new_word = (unsigned char *)malloc(fib_len * 2);
        for (size_t i = 0; i < fib_len; ++i) {
            if (fib_word[i] == 0) {          /* 0 → 01 */
                new_word[new_len++] = 0;
                new_word[new_len++] = 1;
            } else {                         /* 1 → 0 */
                new_word[new_len++] = 0;
            }
        }
        free(fib_word);
        fib_word = new_word;
        fib_len  = new_len;
    }
}

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1)           /* undefined, but harmless */
        return (seq_t)0;

    /* Position in the original Fibonacci word (0‑based). */
    idx_t orig_index = (n - 1) / 3;
    int   pos_in_triplet = (n - 1) % 3;

    /* Ensure the word is long enough. */
    ensure_fib_word((size_t)orig_index);

    unsigned char bit = fib_word[(size_t)orig_index];

    /* Transform the bit: 0 → 100, 1 → 001. */
    if (bit == 0) {   /* 100 */
        return (seq_t)((pos_in_triplet == 0) ? 1 : 0);
    } else {          /* 001 */
        return (seq_t)((pos_in_triplet == 2) ? 1 : 0);
    }
}
