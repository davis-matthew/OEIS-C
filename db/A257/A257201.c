/*
 * A257201  a(n) = n*(n+1)*(n+2)*(n+3)*(n+4)*(n^2+4*n+37)/5040.
 * https://oeis.org/A257201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(n+4)*(n^2+4*n+37)/5040
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* use unsigned 128‑bit for intermediate calculations */
    unsigned __int128 nn = n;
    unsigned __int128 f[6];
    f[0] = nn;
    f[1] = nn + 1;
    f[2] = nn + 2;
    f[3] = nn + 3;
    f[4] = nn + 4;
    f[5] = nn * nn + 4 * nn + 37;

    /* cancel factors of 5040 = 2^4 * 3^2 * 5 * 7 */
    const unsigned int primes[4] = {2, 3, 5, 7};
    int exp[4] = {4, 2, 1, 1};

    for (int i = 0; i < 6; ++i) {
        for (int p = 0; p < 4; ++p) {
            while (exp[p] > 0 && (f[i] % primes[p]) == 0) {
                f[i] /= primes[p];
                --exp[p];
            }
        }
    }

    /* remaining denominator */
    unsigned __int128 denom_rem = 1;
    for (int p = 0; p < 4; ++p) {
        for (int e = 0; e < exp[p]; ++e) {
            denom_rem *= primes[p];
        }
    }

    unsigned __int128 prod = 1;
    for (int i = 0; i < 6; ++i) {
        prod *= f[i];
    }

    prod /= denom_rem;

    return (seq_t)prod;
}
