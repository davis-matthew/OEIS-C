/*
 * A324399  Lexicographically earliest sequence such that a(i) = a(j) => f(i)
 *          = f(j) for all i, j >= 1, where f(n) = A000265(n) for all other
 *          numbers, except that f(n) = 0 if n is an odd prime, and f(1) = -1
 *          and f(2) = -2.
 * https://oeis.org/A324399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rank of first occurrence of the class of f(n); computed
 *            by sieve counting first-occurrence positions (1,2,3,4, odd
 *            composites, 2*odd primes) up to the class's first index
 * Range:     u8 n <= 596, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9237 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    uint64_t m = (uint64_t)n;
    while ((m & 1) == 0) m >>= 1;
    bool mprime = false;
    if (m > 1) {
        mprime = true;
        for (uint64_t d = 3; d * d <= m; d += 2)
            if (m % d == 0) { mprime = false; break; }
    }
    if (m == n && mprime) return (seq_t)3;
    uint64_t F;
    if (m == 1) F = 4;
    else if (mprime) F = 2 * m;
    else F = m;

    unsigned char *comp = (unsigned char *)calloc((size_t)F + 1, 1);
    if (!comp) return (seq_t)0;
    for (uint64_t i = 2; i * i <= F; i++)
        if (!comp[i])
            for (uint64_t j = i * i; j <= F; j += i) comp[j] = 1;
    uint64_t cnt = 4;
    for (uint64_t k = 3; k <= F; k += 2) {
        if (comp[k]) cnt++;            /* odd composite k <= F */
        else if (2 * k <= F) cnt++;    /* odd prime q with 2q <= F */
    }
    free(comp);
    return (seq_t)cnt;
}
