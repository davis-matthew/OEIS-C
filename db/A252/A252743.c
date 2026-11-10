/*
 * A252743  a(n) = A252742(A005940(1+n)).
 * https://oeis.org/A252743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = A252742(A005940(1+n)), where A252742(m) = [A003961(m) >
 *            2*m]. A005940(1+n) has exponent of prime(k) = number of 1 bits
 *            of n followed by k-1 zeros, so the test is: sum over those 1
 *            bits of log(prime(k+1)/prime(k)) > log 2
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 145 terms through n = 7829 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static int A252743_primes[80];
static int A252743_pc = 0;

static void A252743_init(void) {
    if (A252743_pc) return;
    for (int x = 2; x < 500 && A252743_pc < 80; ++x) {
        int ok = 1;
        for (int d = 2; d * d <= x; ++d) if (x % d == 0) { ok = 0; break; }
        if (ok) A252743_primes[A252743_pc++] = x;
    }
}

seq_t a(idx_t n) {
    A252743_init();
    uint64_t m = (uint64_t)n;
    int i = 0; /* zero-based prime index */
    long double sum = 0.0L;
    while (m > 0) {
        if (m & 1) {
            sum += logl((long double)A252743_primes[i + 1]) - logl((long double)A252743_primes[i]);
        } else {
            i++;
        }
        m >>= 1;
    }
    return (sum > logl(2.0L)) ? (seq_t)1 : (seq_t)0;
}
