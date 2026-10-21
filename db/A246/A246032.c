/*
 * A246032  a(n) = A246031(2^n-1).
 * https://oeis.org/A246032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A246031(2^n-1) – computed via the 11‑term linear
 *            recurrence * derived from the generating function * (1 + 6*x -
 *            317*x^2 + 1718*x^3 + 5420*x^4 - 59432*x^5 + 61312*x^6 * +
 *            428928*x^7 - 887296*x^8 - 260096*x^9 + 737280*x^10) * / ((1 -
 *            8*x)*(1 - 12*x - 17*x^2 + 608*x^3 - 856*x^4 - 9920*x^5 * +...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t init[11] = {
    (seq_t)1ULL,          /* a(0) */
    (seq_t)26ULL,         /* a(1) */
    (seq_t)124ULL,        /* a(2) */
    (seq_t)1400ULL,       /* a(3) */
    (seq_t)10000ULL,      /* a(4) */
    (seq_t)89504ULL,      /* a(5) */
    (seq_t)707008ULL,     /* a(6) */
    (seq_t)5924480ULL,    /* a(7) */
    (seq_t)47900416ULL,   /* a(8) */
    (seq_t)393069824ULL,  /* a(9) */
    (seq_t)3189761536ULL /* a(10) */
};

/* Coefficients of the recurrence, in order a(n-1)…a(n-11). */
static const int64_t rec_coef[11] = {
    20,          /* a(n-1) */
   -79,          /* a(n-2) */
  -744,          /* a(n-3) */
  5720,          /* a(n-4) */
  3072,          /* a(n-5) */
 -101936,        /* a(n-6) */
 127616,        /* a(n-7) */
 563968,        /* a(n-8) */
 -1090560,      /* a(n-9) */
 -348160,       /* a(n-10) */
 884736         /* a(n-11) */
};

seq_t a(idx_t n)
{
    if (n < 11U)               /* return the precalculated base cases */
        return init[n];

    /* Working buffer of the last 11 computed values:
     * buf[0] = a(i-10), buf[1] = a(i-9), … , buf[10] = a(i)  */
    seq_t buf[11];
    for (unsigned i = 0; i < 11U; ++i)
        buf[i] = init[i];

    for (idx_t i = 11ULL; i <= n; ++i) {
        seq_t next = (seq_t)0;
        /* compute a(i) = sum_{k=1}^{11} rec_coef[k-1] * a(i-k) */
        for (unsigned k = 0; k < 11U; ++k) {
            seq_t term = buf[10U - k] * (seq_t)rec_coef[k];
            next += term;
        }

        /* shift buffer to the left and insert the new value at the end */
        for (unsigned j = 0; j < 10U; ++j)
            buf[j] = buf[j + 1];
        buf[10U] = next;
    }

    return buf[10U];   /* a(n) */
}
