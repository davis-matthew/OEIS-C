/*
 * A303072  Number of minimal total dominating sets in the n-ladder graph.
 * https://oeis.org/A303072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A253412(n)^2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) satisfies the linear recurrence of order 19 derived from its generating function:  */
/*   a_n = -a_{n-1} + a_{n-2} + 3*a_{n-3} + 7*a_{n-4} + 8*a_{n-5} + 2*a_{n-6} + */
/*          6*a_{n-7} + 6*a_{n-8} - 6*a_{n-11} - 6*a_{n-12} - 2*a_{n-13} - 8*a_{n-14} - */
/*          7*a_{n-15} - 3*a_{n-16} - a_{n-17} + a_{n-18} + a_{n-19}   */
#include <stdbool.h>

/* Initial 19 terms a(1)..a(19) */
static const seq_t init_19[19] = {
    (seq_t)1,    (seq_t)4,    (seq_t)4,    (seq_t)16,
    (seq_t)16,   (seq_t)49,   (seq_t)81,   (seq_t)169,
    (seq_t)324,  (seq_t)625,  (seq_t)1296, (seq_t)2401,
    (seq_t)4900, (seq_t)9409, (seq_t)18769,(seq_t)36481,
    (seq_t)71824,(seq_t)141376,(seq_t)276676
};

/* Recurrence coefficients for a_n = Σ coeff[k]*a_{n-1-k} , k=0..18 */
static const int coeff[19] = {
   -1, 1, 3, 7, 8, 2, 6, 6, 0, 0,
  -6, -6, -2, -8, -7, -3, -1, 1, 1
};

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n <= 19)
        return init_19[n-1];

    /* buffer holds last 19 values a_{k-19} ... a_{k-1} */
    seq_t buf[19];
    for (int i = 0; i < 19; ++i)
        buf[i] = init_19[i];

    /* iterate from n = 20 up to the requested n */
    for (idx_t k = 20; k <= n; ++k) {
        seq_t pos = 0, neg = 0;
        for (int i = 0; i < 19; ++i) {
            seq_t term = buf[18 - i];      /* a_{k-1-i} */
            int c = coeff[i];
            if (c > 0)
                pos += (seq_t)c * term;
            else if (c < 0)
                neg += (seq_t)(-c) * term;
        }
        seq_t next = pos - neg;            /* pos ≥ neg for this sequence */

        /* shift buffer: drop a_{k-19} and append a_{k} */
        for (int i = 0; i < 18; ++i)
            buf[i] = buf[i+1];
        buf[18] = next;
    }

    return buf[18];   /* a(n) */
}
