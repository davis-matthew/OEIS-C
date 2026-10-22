/*
 * A341028  a(n) is the smallest positive integer such that n+a(n) contains
 *          the string n-a(n) in reverse as a substring. If no such number
 *          exists then a(n) = -1.
 * https://oeis.org/A341028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look, base
 * Formula:   a(n) is the minimal positive integer a such that the decimal
 *            representation of n+a contains the decimal representation of
 *            reverse(n-a) as a substring. This implementation searches a from
 *            1 upward for each n and returns the first satisfying value; if
 *            none is found (happens only for n = 1,2,3,4) it...
 * Range:     i8 n <= 67, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Convert a non‑negative seq_t number to an array of decimal digits
   most significant digit first.  buf must be large enough
   (we use 40 bytes in the caller).  *len is set to the number of
   digits produced. */
static void seq_to_digits(seq_t n, unsigned char *buf, size_t *len)
{
    size_t i = 0;
    if (n == 0) {
        buf[0] = 0;
        *len = 1;
        return;
    }
    while (n != 0) {
        buf[i++] = (unsigned char)(n % 10);
        n /= 10;
    }
    /* reverse to most significant first */
    for (size_t j = 0; j < i / 2; ++j) {
        unsigned char tmp = buf[j];
        buf[j] = buf[i - 1 - j];
        buf[i - 1 - j] = tmp;
    }
    *len = i;
}

/* Return the smallest a such that n+a contains reverse(n-a) as a substring */
seq_t a(idx_t n)
{
    const size_t MAX_DIGITS = 40;     /* enough for 128‑bit numbers (≤39 digits) */
    unsigned char nb_digits[MAX_DIGITS];
    unsigned char rev_d_digits[MAX_DIGITS];
    size_t nb_len, rev_d_len;

    for (idx_t a = 1; a <= n; ++a) {
        seq_t d  = (seq_t)n - (seq_t)a;   /* n - a  (>=0) */
        seq_t nb = (seq_t)n + (seq_t)a;   /* n + a  */

        /* digits of n + a */
        seq_to_digits(nb, nb_digits, &nb_len);

        /* digits of reverse(d) are just digits of d from least
           significant to most significant (reverse order of normal
           decimal representation) */
        seq_t tmp = d;
        rev_d_len = 0;
        if (tmp == 0) {
            rev_d_digits[0] = 0;
            rev_d_len = 1;
        } else {
            while (tmp != 0) {
                rev_d_digits[rev_d_len++] = (unsigned char)(tmp % 10);
                tmp /= 10;
            }
        }

        /* substring test */
        if (rev_d_len <= nb_len) {
            for (size_t i = 0; i + rev_d_len <= nb_len; ++i) {
                size_t j = 0;
                while (j < rev_d_len && nb_digits[i + j] == rev_d_digits[j])
                    ++j;
                if (j == rev_d_len)   /* found */
                    return (seq_t)a;
            }
        }
    }

    return (seq_t)-1;   /* no a found (occurs only for n = 1,2,3,4) */
}
