/*
 * A309217  The sequence is {a(n), n>=0}, the concatenation of the binary
 *          expansions of the absolute values |a(n)| is {b(n), n>=0}; start
 *          with a(0)=0; thereafter a(n) = a(n-1)+n if b(n-1)=0, otherwise
 *          a(n) = a(n-1)-n.
 * https://oeis.org/A309217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 29, i16 n <= 719, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A309217:  a(0)=0; for n>=1, a(n)=a(n-1)+n if b(n-1)=0 otherwise a(n)=a(n-1)-n,
   where b is the infinite binary sequence obtained by concatenating the binary
   expansions of |a(k)| (most‑significant bit first), with 0 represented by a
   single 0 bit. */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Handle the first term explicitly. */
    if (n == 0u)
        return (seq_t)0;

    /* Current value of the sequence. */
    seq_t a_prev = (seq_t)0;

    /* Dynamically growable array that stores the bits of b. */
    unsigned char *bits    = (unsigned char*)malloc(1);
    if (!bits) return (seq_t)0;          /* allocation failure → arbitrary value */
    bits[0] = 0;                        /* bits of |a(0)| = 0 */
    size_t bits_len = 1;
    size_t bits_cap = 1;

    for (idx_t i = 1; i <= n; ++i) {
        /* Guiding bit for this step. */
        unsigned char bit = bits[i - 1];

        /* Compute a(i). */
        seq_t inc   = (seq_t)i;
        seq_t a_cur = a_prev + (bit == 0 ? inc : -inc);

        /* Append the binary expansion of |a_cur| to the bit array. */
        seq_t absval = a_cur < (seq_t)0 ? -a_cur : a_cur;
        if (absval == (seq_t)0) {
            /* Representation of 0 is a single 0 bit. */
            if (bits_len + 1 > bits_cap) {
                bits_cap = bits_cap * 2 + 1;
                unsigned char *tmp = (unsigned char*)realloc(bits, bits_cap);
                if (!tmp) { free(bits); return a_cur; }
                bits = tmp;
            }
            bits[bits_len++] = 0;
        } else {
            /* Determine the length of the binary representation. */
            seq_t tmp = absval;
            size_t len = 0;
            while (tmp > (seq_t)0) {
                tmp >>= 1;
                ++len;
            }

            /* Ensure sufficient capacity. */
            if (bits_len + len > bits_cap) {
                bits_cap = bits_cap * 2 + len;
                unsigned char *tmp2 = (unsigned char*)realloc(bits, bits_cap);
                if (!tmp2) { free(bits); return a_cur; }
                bits = tmp2;
            }

            /* Extract bits from most‑significant to least‑significant. */
            for (size_t pos = len; pos > 0; --pos) {
                bits[bits_len++] = (unsigned char)((absval >> (pos - 1)) & (seq_t)1);
            }
        }

        a_prev = a_cur;
    }

    free(bits);
    return a_prev;
}
