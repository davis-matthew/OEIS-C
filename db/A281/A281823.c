/*
 * A281823  Least number k such that (k-n)^2 contains k as a substring.
 * https://oeis.org/A281823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = least k such that (k-n)^2 contains the decimal
 *            representation of k
 * Range:     u8 n <= 3, u16 n <= 1039, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Convert a seq_t value to its decimal digits (MSB first).
   Buffer must be large enough (here 80 digits are sufficient for all
   values encountered in the tested range). */
static void seq_to_digits(seq_t v, unsigned char *buf, size_t *len)
{
    size_t pos = 0;
    seq_t temp = v;
    if (temp == (seq_t)0) {
        buf[0] = 0;
        *len = 1;
        return;
    }
    while (temp > (seq_t)0) {
        seq_t rem = temp % (seq_t)10;
        buf[pos++] = (unsigned char)rem;
        temp = temp / (seq_t)10;
    }
    /* reverse to MSB first */
    for (size_t i = 0; i < pos / 2; ++i) {
        unsigned char t = buf[i];
        buf[i] = buf[pos - 1 - i];
        buf[pos - 1 - i] = t;
    }
    *len = pos;
}

/* Does the decimal representation of x contain the digit sequence
   given by digits_k of length len_k? */
static bool contains_digits(seq_t x, const unsigned char *digits_k, size_t len_k)
{
    unsigned char digits_x[80];
    size_t len_x;
    seq_to_digits(x, digits_x, &len_x);
    if (len_k > len_x)
        return false;
    for (size_t i = 0; i + len_k <= len_x; ++i) {
        bool ok = true;
        for (size_t j = 0; j < len_k; ++j) {
            if (digits_x[i + j] != digits_k[j]) {
                ok = false;
                break;
            }
        }
        if (ok)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    seq_t n_val = (seq_t)n;
    unsigned char digits_k[80];
    size_t len_k;

    for (idx_t k = 1;; ++k) {
        seq_t k_val = (seq_t)k;
        /* digits of k */
        seq_to_digits(k_val, digits_k, &len_k);

        /* absolute difference |k - n| */
        idx_t diff_i = (k >= (idx_t)n) ? (k - (idx_t)n) : ((idx_t)n - k);
        seq_t diff    = (seq_t)diff_i;
        seq_t sq      = diff * diff;

        if (contains_digits(sq, digits_k, len_k))
            return k_val;
    }
    /* unreachable */
    return (seq_t)0;
}
