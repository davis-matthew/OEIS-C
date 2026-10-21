/*
 * A262609  Divisors of 1728.
 * https://oeis.org/A262609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: divisors of 1728 (2^6 * 3^3) in ascending order */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* generate the divisors once and keep them sorted */
    static seq_t divisors[28];
    static bool initialized = false;

    if (!initialized) {
        unsigned idx = 0;
        /* 2^6 * 3^3 = 1728 */
        for (unsigned i = 0; i <= 6; ++i) {
            for (unsigned j = 0; j <= 3; ++j) {
                seq_t d = (seq_t)1;
                /* multiply by 2^i */
                seq_t p = (seq_t)1;
                for (unsigned k = 0; k < i; ++k)
                    p *= (seq_t)2;
                d *= p;
                /* multiply by 3^j */
                p = (seq_t)1;
                for (unsigned k = 0; k < j; ++k)
                    p *= (seq_t)3;
                d *= p;
                divisors[idx++] = d;
            }
        }
        /* insertion sort */
        for (unsigned i = 1; i < 28; ++i) {
            seq_t key = divisors[i];
            unsigned j = i;
            while (j > 0 && divisors[j - 1] > key) {
                divisors[j] = divisors[j - 1];
                --j;
            }
            divisors[j] = key;
        }
        initialized = true;
    }

    if (n < 1 || n > 28)
        return (seq_t)0;     /* undefined beyond the finite sequence */

    return divisors[n - 1];
}
