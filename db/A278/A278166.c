/*
 * A278166  a(n) = number of integers one more than a prime encountered before
 *          reaching 0 when starting from k = ((n+1)^2)-1 and iterating map k
 *          -> k - A002828(k).
 * https://oeis.org/A278166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes among {k-1} over the orbit of k -> k -
 *            A002828(k) from k=(n+1)^2-1 down to 0 (sieve-based simulation)
 * Range:     u8 n <= 65, u16 n <= 1510, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 92 terms through n = 9288 (2026-11-08)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isqrt_u64.h"

#define A278166_GET(B, i) (((B)[(i) >> 3] >> ((i) & 7)) & 1)
#define A278166_SET(B, i) ((B)[(i) >> 3] |= (uint8_t)(1u << ((i) & 7)))

seq_t a(idx_t n) {
    uint64_t K = ((uint64_t)n + 1) * ((uint64_t)n + 1) - 1;
    size_t nb = (size_t)(K / 8 + 2);
    uint8_t *comp = (uint8_t *)calloc(nb, 1);
    uint8_t *two = (uint8_t *)calloc(nb, 1);
    uint64_t i, j, k;
    uint64_t cnt = 0;
    if (!comp || !two) { free(comp); free(two); return (seq_t)0; }

    /* composite sieve (odd multiples) */
    for (i = 3; i * i <= K; i += 2) {
        if (!A278166_GET(comp, i)) {
            for (j = i * i; j <= K; j += 2 * i) A278166_SET(comp, j);
        }
    }
    /* sums of two squares */
    for (i = 0; i * i <= K; i++) {
        for (j = i; i * i + j * j <= K; j++) {
            uint64_t v = i * i + j * j;
            A278166_SET(two, v);
        }
    }

    k = K;
    while (k > 0) {
        uint64_t m = k - 1;
        bool pr;
        uint64_t s, r, t;
        if (m < 2) pr = false;
        else if (m == 2) pr = true;
        else if ((m & 1) == 0) pr = false;
        else pr = !A278166_GET(comp, m);
        if (pr) cnt++;

        r = isqrt_u64(k);
        if (r * r == k) s = 1;
        else if (A278166_GET(two, k)) s = 2;
        else {
            t = k;
            while ((t & 3) == 0) t >>= 2;
            s = ((t & 7) == 7) ? 4 : 3;
        }
        k -= s;
    }
    free(comp);
    free(two);
    return (seq_t)cnt;
}
