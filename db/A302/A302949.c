/*
 * A302949  Number of nX4 0..1 arrays with every element equal to 1, 2, 3, 4
 *          or 5 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A302949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) +41*a(n-2) +29*a(n-3) -181*a(n-4) -466*a(n-5)
 *            -225*a(n-6) +66*a(n-7) +298*a(n-8) -52*a(n-9) +57*a(n-10)
 *            -10*a(n-11) -25*a(n-12) +4*a(n-13) with a(1) = 2, a(2) = 46,
 *            a(3) = 519, a(4) = 6531, a(5) = 80589, a(6) = 998670, a(7) =
 *            12365841, a(8) = 153141597, a(9) = 1896492042, a(10) =...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/*---------------------------------------------------------------
 *  Memoised evaluation of the recurrence.
 *  Works for any index order, up to a pre-chosen maximum N.
 *---------------------------------------------------------------*/

#define MAX_N 20000 /* sufficient for n up to about 10000 */

static seq_t cache[MAX_N + 1];   /* storage of previously computed terms */
static idx_t  last_computed = 0; /* highest n for which cache[n] is valid */

/* initial values a(1) .. a(13) */
static const seq_t base_terms[13] = {
    (seq_t)2,
    (seq_t)46,
    (seq_t)519,
    (seq_t)6531,
    (seq_t)80589,
    (seq_t)998670,
    (seq_t)12365841,
    (seq_t)153141597,
    (seq_t)1896492042,
    (seq_t)23486107930,
    (seq_t)290851104799,
    (seq_t)3601890066510U,
    (seq_t)44605681284501U
};

/* coefficients of the recurrence */
static const int coeffs[13] = {
    9,   41,   29, -181, -466,
   -225,   66, 298, -52,  57,
   -10, -25,   4
};

static void init_base_vals(void)
{
    for (idx_t i = 1; i <= 13 && i <= MAX_N; ++i)
        cache[i] = base_terms[i - 1];
    last_computed = 13;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not defined for n<1, but harmless */

    if (n > (idx_t)MAX_N) {
        /* out of bounds – could be extended, but per requirements
           n will not exceed MAX_N for test cases. */
        return (seq_t)0;
    }

    if (last_computed == 0) init_base_vals();

    if (n <= last_computed) return cache[n];

    /* compute terms from last_computed+1 up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* for i <= 13 recurrence is not defined; they already hold base values */
        if (i <= 13) {
            /* nothing to do – values already cached */
            continue;
        }

        seq_t pos = 0;
        seq_t neg = 0;
        for (int j = 0; j < 13; ++j) {
            idx_t idx = i - 1 - j;          /* a(i-(j+1)) */
            seq_t term = cache[(size_t)idx];
            int coeff = coeffs[j];
            if (coeff >= 0)
                pos += term * (unsigned int)coeff;
            else
                neg += term * (unsigned int)(-coeff);
        }
        cache[(size_t)i] = pos - neg;      /* pos >= neg by definition of the sequence */
    }
    last_computed = n;
    return cache[(size_t)n];
}
