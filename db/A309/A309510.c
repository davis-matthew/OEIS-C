/*
 * A309510  Divisors of 196883.
 * https://oeis.org/A309510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 196883.
   a(1)=1, a(2)=47, a(3)=59, a(4)=71,
   a(5)=2773, a(6)=3337, a(7)=4189, a(8)=196883.
   For n>8 the sequence is undefined; here we return 0. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1 || n > 8) /* undefined indices */
        return 0;

    /* Memoize the sorted divisors once. */
    static seq_t cache[8];
    static bool init = false;

    if (!init) {
        const seq_t primes[3] = {47, 59, 71};
        seq_t vals[8];
        int idx = 0;

        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                for (int k = 0; k < 2; ++k) {
                    seq_t d = 1;
                    if (i) d *= primes[0];
                    if (j) d *= primes[1];
                    if (k) d *= primes[2];
                    vals[idx++] = d;
                }

        /* Sort ascending */
        for (int i = 0; i < 8; ++i)
            for (int j = i + 1; j < 8; ++j)
                if (vals[j] < vals[i]) {
                    seq_t tmp = vals[i];
                    vals[i] = vals[j];
                    vals[j] = tmp;
                }

        for (int i = 0; i < 8; ++i)
            cache[i] = vals[i];
        init = true;
    }

    return cache[n - 1];
}
