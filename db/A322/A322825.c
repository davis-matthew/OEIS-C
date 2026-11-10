/*
 * A322825  A variant of A322827.
 * https://oeis.org/A322825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   write the binary expansion of n as runs L[0], L[1], ..., L[m-1]
 *            (from the least significant bit); T[0] = L[0], T[i] = L[i]-1 for
 *            i > 0; c[i] = T[0] + ... + T[i]; a(n) = Product_{i=0..m-1}
 *            prime(i+1)^c[i]; a(0) = 1. Hence A001221(a(n)) = A005811(n),
 *            A001222(a(n)) = A227183(n)
 * Range:     u8 n <= 8, u16 n <= 32, u32 n <= 82, u64 n <= 583,
 *            u128 n <= 5263, big any n
 * Validated: standard, pass, 68 terms through n = 9286 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static unsigned A322825_nth_prime(unsigned k)
{
    unsigned cnt = 0;
    unsigned c = 1;
    while (cnt < k) {
        c++;
        bool pr = true;
        for (unsigned d = 2; d * d <= c; d++) {
            if (c % d == 0) { pr = false; break; }
        }
        if (pr) cnt++;
    }
    return c;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    if (n == 0)
        return result;

    unsigned long long cum = 0;
    unsigned runIndex = 0;
    unsigned pidx = 0;
    uint64_t m = n;

    while (m > 0) {
        uint64_t bit = m & 1ULL;
        unsigned long long len = 0;
        while (m > 0 && (m & 1ULL) == bit) {
            len++;
            m >>= 1;
        }
        unsigned long long t = len;
        if (runIndex > 0)
            t -= 1;
        cum += t;
        pidx++;
        unsigned p = A322825_nth_prime(pidx);
        for (unsigned long long e = 0; e < cum; e++)
            result *= (seq_t)p;
        runIndex++;
    }
    return result;
}
