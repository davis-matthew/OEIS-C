/*
 * A395553  Number of no-overhang stacked partitions (teeth) of base 4 and
 *          total size n.
 * https://oeis.org/A395553
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of q^n in h_4(q) =
 *            q^4*(1+q)*(1+2q+q^3)/((1-q)*(1-q^2)*(1-q^3)*(1-q^4)) The
 *            numerator expands to q^4+3q^5+2q^6+q^7+q^8. Let s_n be the
 *            coefficient of q^n in 1/((1-q)*(1-q^2)*(1-q^3)*(1-q^4)), then
 *            s_n satisfies s_0 = 1, s_n = s_{n-1} + s_{n-2} - 2*s_{n-5} +
 *            s_{n-8} + s_{n-9} -...
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)0;             /* undefined for n<4, return 0 */

    /* static arrays for memoisation */
    static seq_t *s_arr = NULL;        /* s_n coefficients */
    static seq_t *a_arr = NULL;        /* a_n values */
    static idx_t cur_max = 0;          /* highest n already computed */

    /* ensure arrays allocated up to n */
    if (n > cur_max) {
        size_t newsize = (size_t)(n + 1);

        /* reallocate */
        seq_t *tmp;
        tmp = (seq_t *)realloc(s_arr, newsize * sizeof(seq_t));
        s_arr = tmp;
        tmp = (seq_t *)realloc(a_arr, newsize * sizeof(seq_t));
        a_arr = tmp;

        /* initialise new entries: zero them (calloc or explicit loop) */
        for (idx_t i = cur_max + 1; i < newsize; ++i) {
            s_arr[i] = 0;
            a_arr[i] = 0;
        }

        /* initialise base for s array */
        if (cur_max == 0) {
            s_arr[0] = (seq_t)1;       /* s_0 = 1 */
        }

        /* compute new terms from cur_max+1 up to n */
        for (idx_t i = cur_max + 1; i <= n; ++i) {
            /* compute s_i */
            seq_t val = 0;
            if (i >= 1) val += s_arr[i - 1];
            if (i >= 2) val += s_arr[i - 2];
            if (i >= 5) val -= (seq_t)2 * s_arr[i - 5];
            if (i >= 8) val += s_arr[i - 8];
            if (i >= 9) val += s_arr[i - 9];
            if (i >= 10) val -= s_arr[i - 10];
            s_arr[i] = val;

            /* compute a_i using the convolution with the numerator coefficients */
            seq_t a_val = 0;
            if (i >= 4) a_val += s_arr[i - 4];
            if (i >= 5) a_val += (seq_t)3 * s_arr[i - 5];
            if (i >= 6) a_val += (seq_t)2 * s_arr[i - 6];
            if (i >= 7) a_val += s_arr[i - 7];
            if (i >= 8) a_val += s_arr[i - 8];
            a_arr[i] = a_val;
        }

        cur_max = n;
    }

    return a_arr[n];
}
