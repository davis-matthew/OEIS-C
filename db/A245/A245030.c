/*
 * A245030  Divisors of 7^24 - 1.
 * https://oeis.org/A245030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th smallest divisor of 7^24-1
 * Range:     u8 n <= 74, u16 n <= 714, u32 n <= 7061, u64 n <= 16119,
 *            u128 n <= 16128, big any n
 * Validated: full, pass, 16128 terms through n = 16128 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A245030: the divisors of 7^24 - 1 = 2^6 3^2 5^2 13 19 43 73 181 193 409 1201,
 * in increasing order. 7^24 - 1 is about 1.9e20, above 2^64, so the divisors
 * are generated and sorted in 128 bits and only the one asked for is cast to
 * seq_t (sorting wrapped values put them out of order). */

#include <stdlib.h>
#include <stdint.h>

typedef unsigned __int128 u128;

static int cmp_u128(const void *a, const void *b)
{
    u128 x = *(const u128 *)a, y = *(const u128 *)b;
    return (x > y) - (x < y);
}

static void gen(int idx, u128 cur, const unsigned primes[], const unsigned exps[],
                int num_primes, u128 *divs, size_t *pos)
{
    if (idx == num_primes) {
        divs[(*pos)++] = cur;
        return;
    }
    for (unsigned e = 0; e <= exps[idx]; ++e) {
        gen(idx + 1, cur, primes, exps, num_primes, divs, pos);
        cur *= primes[idx];
    }
}

seq_t a(idx_t n)
{
    static u128 *divs = NULL;
    static size_t div_count = 0;
    if (!divs) {
        static const unsigned primes[] = {2, 3, 5, 13, 19, 43, 73, 181, 193, 409, 1201};
        static const unsigned exps[] = {6, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1};
        const int num_primes = sizeof primes / sizeof primes[0];
        size_t count = 1;
        for (int i = 0; i < num_primes; ++i) count *= exps[i] + 1;
        divs = (u128 *)malloc(count * sizeof *divs);
        if (!divs) return (seq_t)0;
        size_t pos = 0;
        gen(0, 1, primes, exps, num_primes, divs, &pos);
        qsort(divs, count, sizeof *divs, cmp_u128);
        div_count = count;
    }
    if (n < 1 || (size_t)n > div_count) return (seq_t)0;
    return (seq_t)divs[n - 1];
}
