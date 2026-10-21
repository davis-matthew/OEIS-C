/*
 * A290741  Orbit of word "2" under the 3-shift tag system over the alphabet
 *          {1,2} defined in the Comments.
 * https://oeis.org/A290741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth word in the 3‑shift tag system over {1,2}. Begin
 *            with w₁ = "2". For each step k>1, if the current word starts
 *            with '1', replace it by wₖ + "2112" and delete the first three
 *            characters; otherwise replace it by wₖ + "1221222" and delete
 *            the first three characters. The returned value...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* not used – offset is 1 */
    if (n == 1) return (seq_t)2;   /* a(1) = 2 */

    /* Number of transformations (steps) */
    size_t steps = (size_t)(n - 1);

    /* Allocate enough space for all characters that may ever be written.
       At most 7 new characters are appended each step. */
    size_t buf_size = 1 + 7 * steps;
    char *buf = (char *)malloc(buf_size);
    if (!buf) return 0;            /* out of memory: return 0 */

    /* Initialise with the first word "2" */
    buf[0] = '2';
    size_t start   = 0;   /* index of the first character in the current word */
    size_t cur_len = 1;   /* current word length */

    for (size_t i = 0; i < steps; ++i) {
        char first = buf[start];
        const char *append;
        size_t append_len;

        if (first == '1') {
            append     = "2112";
            append_len = 4;
        } else {
            append     = "1221222";
            append_len = 7;
        }

        /* Append the new segment */
        size_t pos = start + cur_len;          /* where new data starts */
        for (size_t j = 0; j < append_len; ++j) {
            buf[pos + j] = append[j];
        }

        /* Update lengths after the append and the removal of the first three
           characters. */
        cur_len += append_len;  /* after append */
        start   += 3;           /* drop first 3 */
        cur_len -= 3;
    }

    /* Convert the final word (a(n)) to a decimal integer */
    seq_t value = 0;
    for (size_t i = 0; i < cur_len; ++i) {
        value = value * (seq_t)10 + (seq_t)(buf[start + i] - '0');
    }

    free(buf);
    return value;
}
