/*
 * A286939  Fixed point of the mapping 00->001, 1->000, starting with 00.
 * https://oeis.org/A286939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth bit (1‑indexed) of the infinite word produced by
 *            repeatedly scanning the current word from left to right and
 *            replacing: an occurrence of the pattern "00" → "001" a single
 *            "1" → "000" a lone "0" → "0" starting from the string "00". The
 *            sequence a(n) is the nth symbol of the fixed po...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *buffer = NULL;   /* dynamically grown word */
static idx_t  buf_len = 0;     /* current number of stored bits */
static idx_t  buf_cap = 0;     /* allocated capacity */

/* initialise the word with the start string "00" */
static void init_buffer(void) {
    if (buffer == NULL) {
        buf_cap = 4;
        buffer = (seq_t *)malloc(sizeof(seq_t) * buf_cap);
        buffer[0] = (seq_t)0;
        buffer[1] = (seq_t)0;
        buf_len = 2;
    }
}

/* grow the word until it contains at least 'target' bits */
static void grow_to(idx_t target) {
    init_buffer();
    while (buf_len < target) {
        /* generate the next word by the rules described above */
        idx_t new_cap = buf_len * 3 + 10;
        seq_t *new_buf = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        idx_t new_len = 0;
        idx_t i;
        for (i = 0; i < buf_len; ++i) {
            if (buffer[i] == (seq_t)0 &&
                i + 1 < buf_len && buffer[i + 1] == (seq_t)0) {
                /* replace "00" → "001" */
                new_buf[new_len++] = (seq_t)0;
                new_buf[new_len++] = (seq_t)0;
                new_buf[new_len++] = (seq_t)1;
                ++i;  /* skip the next character, which has just been processed */
            } else if (buffer[i] == (seq_t)1) {
                /* replace "1" → "000" */
                new_buf[new_len++] = (seq_t)0;
                new_buf[new_len++] = (seq_t)0;
                new_buf[new_len++] = (seq_t)0;
            } else {
                /* lone "0" stays as "0" */
                new_buf[new_len++] = (seq_t)0;
            }
        }
        free(buffer);
        buffer   = new_buf;
        buf_len  = new_len;
        buf_cap  = new_cap;
    }
}

/* public function required by the ois‑seq framework */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for n<1, return 0 as a safe value */
    grow_to(n);                    /* ensure we have at least n bits */
    return buffer[n - 1];          /* nth bit, 1‑indexed */
}
