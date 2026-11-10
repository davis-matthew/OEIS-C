/*
 * A322589  Lexicographically earliest such sequence a that for all i, j, a(i)
 *          = a(j) => f(i) = f(j), where f(n) = 0 for odd primes, and f(n) =
 *          A007913(n) for any other number.
 * https://oeis.org/A322589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct classes of f whose first occurrence is
 *            <= the first occurrence of f(n)'s class; computed by a sieve.
 *            First occurrence: 3 for f=0 (odd primes), 4p for squarefree part
 *            an odd prime p, else the squarefree part itself
 * Range:     u8 n <= 412, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8953 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t A322589_sqfree(idx_t n) {
    seq_t result = 1;
    seq_t temp = (seq_t)n;
    seq_t i = 2;
    while (i <= temp / i) {
        seq_t i_sq = i * i;
        while (temp % i_sq == 0) {
            temp = temp / i_sq;
        }
        if (temp % i == 0) {
            result = result * i;
            temp = temp / i;
        }
        i++;
    }
    if (temp > 1) {
        result = result * temp;
    }
    return result;
}

static bool A322589_isprime(uint64_t x) {
    if (x < 2) return false;
    for (uint64_t d = 2; d * d <= x; d++) {
        if (x % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    uint64_t T;
    uint64_t nn = (uint64_t)n;
    if (nn > 2 && A322589_isprime(nn)) {
        T = 3;
    } else {
        uint64_t s = (uint64_t)A322589_sqfree(n);
        if (s > 2 && A322589_isprime(s)) T = 4 * s;
        else T = s;
    }
    /* sieve up to T */
    unsigned char *comp = (unsigned char *)calloc((size_t)T + 1, 1);
    unsigned char *nsf = (unsigned char *)calloc((size_t)T + 1, 1);
    for (uint64_t i = 2; i <= T; i++) {
        if (!comp[i]) {
            for (uint64_t j = i * 2; j <= T; j += i) comp[j] = 1;
            if (i <= T / i) {
                uint64_t sq = i * i;
                for (uint64_t j = sq; j <= T; j += sq) nsf[j] = 1;
            }
        }
    }
    uint64_t cnt = 0;
    for (uint64_t m = 1; m <= T; m++) {
        if (m <= 3) { cnt++; continue; }
        if (!comp[m]) continue;            /* prime > 3: class 0 */
        if (!nsf[m]) { cnt++; continue; }  /* squarefree composite */
        if (m % 4 == 0) {
            uint64_t p = m / 4;
            if (p > 2 && !comp[p]) cnt++;
        }
    }
    free(comp);
    free(nsf);
    return (seq_t)cnt;
}
