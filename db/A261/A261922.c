/*
 * A261922  a(n) = smallest nonnegative number that is not a substring of n in
 *          its binary representation.
 * https://oeis.org/A261922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest nonnegative integer that is not a substring * of
 *            n in its binary representation. The function uses a brute‑force
 *            * search; for each n it builds its binary string and checks
 *            successive * integers m for substring inclusion until it finds a
 *            missing one
 * Range:     u8 n <= 3712, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include <stddef.h>

/* Convert a seq_t value to a binary string.  The resulting string
 * is stored in buf and its length in *len.  The buffer must have
 * space for at least 128 characters (more than enough for all cases). */
static void seq_to_bin(seq_t x, char *buf, size_t *len)
{
    if (x == (seq_t)0) {
        buf[0] = '0';
        *len = 1;
        return;
    }

    size_t i = 0;
    /* Build the string in reverse order. */
    while (x != (seq_t)0) {
        buf[i++] = (x & (seq_t)1) ? '1' : '0';
        x >>= 1;
    }
    /* Reverse it to obtain the standard binary representation. */
    for (size_t j = 0; j < i / 2; ++j) {
        char t = buf[j];
        buf[j] = buf[i - 1 - j];
        buf[i - 1 - j] = t;
    }
    *len = i;
}

seq_t a(idx_t n)
{
    /* Build binary representation of n once. */
    char bin_n[128];
    size_t len_n;
    seq_to_bin((seq_t)n, bin_n, &len_n);
    bin_n[len_n] = '\0'; /* Ensure null-termination for strstr. */

    /* Search for the smallest missing nonnegative integer. */
    for (idx_t m = 0; ; ++m) {
        char bin_m[128];
        size_t len_m;
        seq_to_bin((seq_t)m, bin_m, &len_m);
        bin_m[len_m] = '\0';
        /* strstr requires null-terminated strings. */
        if (strstr(bin_n, bin_m) == NULL) {
            return (seq_t)m;
        }
    }
}
