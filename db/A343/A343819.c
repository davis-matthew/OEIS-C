/*
 * A343819  Numbers k such that k and k+1 have the same number of Fermi-Dirac
 *          factors (A064547).
 * https://oeis.org/A343819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A343819 – Numbers k such that k and k+1 have the same number of Fermi‑Dirac factors (A064547).
    Algorithm: iterate natural numbers, compute A064547 for each, and record k when A(k)=A(k+1).
    The function returns the n‑th such number (1‑based indexing). */
#include <stdint.h>

static seq_t compute_A(uint64_t n)
{
    seq_t sum = 0;
    int e;
    /* factor 2 */
    if ((n & 1ULL) == 0) {
        e = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            ++e;
        }
        sum += __builtin_popcount((unsigned)e);
    }
    /* odd factors */
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            sum += __builtin_popcount((unsigned)e);
        }
    }
    /* remaining prime factor */
    if (n > 1) {
        /* exponent is 1, popcount(1) = 1 */
        sum += 1;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n==0, return 0 as placeholder */

    idx_t count = 0;
    seq_t prevA = compute_A(1);   /* A(1) = 0 */

    for (idx_t k = 1; ; ++k) {
        seq_t curA = compute_A(k + 1);   /* A(k+1) */
        if (prevA == curA) {
            ++count;
            if (count == n)
                return (seq_t)k;         /* k is the required term */
        }
        prevA = curA;
    }
}
