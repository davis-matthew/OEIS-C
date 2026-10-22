/*
 * A362689  Binomial(n+p, n) mod n where p=9.
 * https://oeis.org/A362689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+9, n) mod n
 * Range:     u8 n <= 299, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute using cancelling of factors to avoid overflow. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/_gcd64.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)0;          /* binomial(10,1)=10, 10 mod 1 = 0 */
    /* Prepare numerators (n+1 .. n+9) and denominators (1 .. 9) */
    uint64_t numer[9];
    uint64_t denom[9];
    for (int i = 0; i < 9; ++i) {
        numer[i] = (uint64_t)n + (uint64_t)(i + 1);
        denom[i] = (uint64_t)(i + 1);
    }
    /* Cancel gcds between numerators and denominators */
    for (int i = 0; i < 9; ++i) {
        uint64_t d = denom[i];
        if (d == 1) continue;
        for (int j = 0; j < 9 && d > 1; ++j) {
            uint64_t g = _gcd64(numer[j], d);
            if (g > 1) {
                numer[j] /= g;
                d /= g;
            }
        }
        denom[i] = d;   /* should be 1 if cancellation succeeded fully */
    }
    /* Compute product of remaining numerators modulo n */
    /* Use 128-bit multiplication to avoid overflow on uint64_t seq_t compilers */
    unsigned __int128 mod = (unsigned __int128)n;
    unsigned __int128 res = 1 % mod;
    for (int i = 0; i < 9; ++i) {
        unsigned __int128 mul = (unsigned __int128)(numer[i] % (uint64_t)n);
        res = (res * mul) % mod;
    }
    return (seq_t)res;
}
