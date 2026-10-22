/*
 * A322409  Compound tribonacci sequence with a(n) = A278040(A278040(n)), for
 *          n >= 0.
 * https://oeis.org/A322409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(A(n)+B(n)) + n + 3 where A(n) = n‑th zero position in
 *            the tribonacci ternary word (A278040), B(n) = n‑th one position
 *            (A278039). This is the compound tribonacci sequence with offset
 *            0
 * Range:     u8 n <= 21, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Static data for the tribonacci construction. */
static unsigned char *word = NULL;       /* current tribonacci word */
static size_t word_len = 0;              /* length of current word */

static idx_t *zero_pos = NULL;           /* positions of zeros */
static idx_t *one_pos  = NULL;           /* positions of ones  */
static size_t zero_cap = 0, zero_len = 0;
static size_t one_cap  = 0, one_len  = 0;

/* Initialises the starting word if necessary. */
static void init_word(void)
{
    if (!word) {
        word_len = 1;
        word = (unsigned char *)malloc(word_len);
        word[0] = 0;           /* w0 = "0" */
    }
}

/* Builds the position arrays from the current word. */
static void recompute_pos(void)
{
    zero_len = 0;
    one_len  = 0;

    for (size_t i = 0; i < word_len; ++i) {
        unsigned char c = word[i];
        if (c == 0) {
            if (zero_len == zero_cap) {
                zero_cap = zero_cap ? zero_cap * 2 : 16;
                zero_pos = (idx_t *)realloc(zero_pos, zero_cap * sizeof(idx_t));
            }
            zero_pos[zero_len++] = (idx_t)i;
        } else if (c == 1) {
            if (one_len == one_cap) {
                one_cap = one_cap ? one_cap * 2 : 16;
                one_pos = (idx_t *)realloc(one_pos, one_cap * sizeof(idx_t));
            }
            one_pos[one_len++] = (idx_t)i;
        }
    }
}

/* Generates the next tribonacci word from the current one. */
static void generate_next(void)
{
    size_t new_len = 0;
    for (size_t i = 0; i < word_len; ++i) {
        unsigned char c = word[i];
        if (c == 0 || c == 1)
            new_len += 2;
        else
            new_len += 1;
    }

    unsigned char *next = (unsigned char *)malloc(new_len);
    size_t pos = 0;
    for (size_t i = 0; i < word_len; ++i) {
        unsigned char c = word[i];
        if (c == 0) {
            next[pos++] = 0;
            next[pos++] = 1;
        } else if (c == 1) {
            next[pos++] = 0;
            next[pos++] = 2;
        } else {            /* c == 2 */
            next[pos++] = 0;
        }
    }

    free(word);
    word   = next;
    word_len = new_len;
}

/* Makes sure that the zero and one position arrays contain at least n+1
   entries so that a(n) can be computed. */
static void ensure_precompute(idx_t n)
{
    init_word();

    while (zero_len <= (size_t)n || one_len <= (size_t)n) {
        generate_next();
        recompute_pos();
    }
}

/* Public function for the OEIS sequence. */
seq_t a(idx_t n)
{
    ensure_precompute(n);

    seq_t zn = (seq_t)zero_pos[(size_t)n];
    seq_t on = (seq_t)one_pos[(size_t)n];

    return (seq_t)2 * (zn + on) + (seq_t)n + (seq_t)3;
}
