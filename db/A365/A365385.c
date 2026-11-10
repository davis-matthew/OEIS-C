/*
 * A365385  a(n) = A331410(A163511(n)).
 * https://oeis.org/A365385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A331410(A163511(n)); A163511(n) is kept as a
 *            prime-exponent vector over the first 20 primes, then the map k
 *            -> k + k/lpf(k) is iterated until only the prime 2 remains
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 122 terms through n = 8498 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const int A365385_pr[20] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71
};

seq_t a(idx_t n)
{
    int ex[20];
    int i;
    for (i = 0; i < 20; ++i) ex[i] = 0;
    if (n == 0) return (seq_t)0;

    /* Build exponent vector of A163511(n) */
    int cnt = 0;
    int prev = 0, bitpos = 1;
    idx_t tmp = n;
    while (tmp) {
        if (tmp & 1) {
            if (cnt >= 20) return (seq_t)0; /* beyond supported range */
            ex[cnt] += bitpos - prev - 1;
            prev = bitpos;
            ++cnt;
        }
        ++bitpos;
        tmp >>= 1;
    }
    ex[cnt - 1] += 1; /* factor p(r) */

    seq_t steps = 0;
    for (;;) {
        int top = 19;
        while (top > 0 && ex[top] == 0) --top;
        if (top == 0) break; /* only powers of 2 left */
        int p = A365385_pr[top];
        ex[top]--;
        /* multiply by (p+1) */
        int m = p + 1;
        for (i = 0; i < 20 && m > 1; ++i) {
            while (m % A365385_pr[i] == 0) {
                m /= A365385_pr[i];
                ex[i]++;
            }
        }
        ++steps;
    }
    return steps;
}
