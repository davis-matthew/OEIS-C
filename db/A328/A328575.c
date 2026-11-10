/*
 * A328575  a(n) = A003557(A032742(A276086(n))).
 * https://oeis.org/A328575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003557(A032742(A276086(n))); A276086(n) = prod
 *            prime(k+1)^d_k with d_k the primorial-base digits of n, computed
 *            here from the exponent vector
 * Range:     u8 n <= 28, u16 n <= 148, u32 n <= 1048, u64 n <= 2310,
 *            u128 n <= 2310, big any n
 * Validated: standard, pass, 107 terms through n = 2276 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const unsigned smallprimes_a328575[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97
};

seq_t a(idx_t n)
{
    uint64_t e[32];
    int cnt = 0;
    uint64_t r = (uint64_t)n;
    int i;

    while (r > 0 && cnt < 25) {
        uint64_t p = smallprimes_a328575[cnt];
        e[cnt++] = r % p;
        r /= p;
    }

    /* A032742: divide by the smallest prime factor */
    for (i = 0; i < cnt; i++) {
        if (e[i] > 0) {
            e[i]--;
            break;
        }
    }

    /* A003557: each exponent e becomes e-1 (for e >= 1) */
    seq_t result = 1;
    for (i = 0; i < cnt; i++) {
        if (e[i] > 1) {
            uint64_t j;
            for (j = 1; j < e[i]; j++)
                result *= (seq_t)smallprimes_a328575[i];
        }
    }
    return result;
}
