/*
 * A374047  Numbers k for which A328845(k) is odd, where A328845 is the first
 *          Fibonacci based variant of arithmetic derivative.
 * https://oeis.org/A374047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 149, u16 n <= 9249, u32 n <= 9249, u64 n <= 9249,
 *            u128 n <= 9249, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A374047: numbers k for which the Fibonacci-based arithmetic derivative
   A328845(k) is odd.  The function returns the n-th such integer (offset 1). */

#include <stdbool.h>
#include <stdint.h>

/* Smallest prime factor of n.  For n ≤ 1 the result is n itself. */
static uint64_t smallest_prime_factor(uint64_t n) {
    if (n <= 1) return n;
    if ((n & 1ULL) == 0) return 2ULL;
    for (uint64_t i = 3ULL; i * i <= n; i += 2ULL) {
        if (n % i == 0) return i;
    }
    return n;
}

/* Returns true iff the Fibonacci-based arithmetic derivative A328845(n)
   is odd.  Computation is performed modulo 2 using the recursive rule
   D(ab) = a*D(b) + b*D(a). */
static bool derivative_odd(uint64_t n) {
    if (n <= 1) return false;          /* D(1)=0 */
    uint64_t p = smallest_prime_factor(n);
    if (p == n) {                      /* n is prime */
        return (p != 3ULL);            /* F_p odd except for p=3 */
    }
    /* Factor out p^e */
    uint64_t e = 0;
    uint64_t tmp = n;
    while ((tmp % p) == 0) { tmp /= p; ++e; }
    uint64_t m = tmp;                  /* remaining cofactor, < n */

    /* term1 = (p^e mod 2) * D(m)  modulo 2 */
    bool term1 = false;
    if ((p & 1ULL) != 0) {             /* p is odd => p^e is odd */
        term1 = derivative_odd(m);
    }

    /* term2 = (m mod 2) * D(p^e)  modulo 2 */
    bool term2 = false;
    if ((m & 1ULL) != 0) {             /* m is odd */
        bool dpe;
        if (p == 2ULL) {
            dpe = (e == 1);            /* only 2^1 contributes odd part */
        } else if (p == 3ULL) {
            dpe = false;               /* F_3 is even => no odd part */
        } else {
            dpe = ((e & 1ULL) != 0);   /* odd prime, odd part if exp odd */
        }
        term2 = dpe;
    }

    return term1 ^ term2;               /* addition modulo 2 = XOR */
}

seq_t a(idx_t n) {
    idx_t count = 0;
    for (idx_t k = 2; ; ++k) {
        if (derivative_odd((uint64_t)k)) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
    }
}
