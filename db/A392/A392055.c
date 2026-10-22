/*
 * A392055  a(n) = p if n = p^k is a perfect power of either a ramified or
 *          inert prime in the Gaussian integers and 1 otherwise.
 * https://oeis.org/A392055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p if n = p^k is a perfect power of either a ramified or
 *            inert prime in the Gaussian integers and 1 otherwise
 * Range:     u8 n <= 262, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* static helper: smallest prime factor (SPF) of n (n >= 1) */
static uint64_t spf(uint64_t n) {
    if (n <= 2) return n;                /* n is 1 or 2 */
    if (n % 2ULL == 0ULL) return 2ULL;
    for (uint64_t i = 3ULL; i * i <= n; i += 2ULL)
        if (n % i == 0ULL) return i;
    return n;                             /* n is prime */
}

seq_t a(idx_t n) {
    /* n is the index of the sequence, and also the value we test
     * for being a prime power.  We return the base prime p if
     * n is a power of a ramified (p == 2) or inert (p % 4 == 3)
     * prime; otherwise we return 1.  Index 0 is ignored; for n=1
     * the result is 1.  All arithmetic that could overflow Standard
     * 64‑bit types is avoided by using 64‑bit integers for n, which
     * fits the required range (n <= 10 000 for typical tests). */
    if (n <= 1)
        return (seq_t)1;

    uint64_t nn = (uint64_t)n;
    uint64_t p  = spf(nn);

    /* Confirm that n is a pure power of p */
    uint64_t t = nn;
    while (t % p == 0ULL) t /= p;
    if (t != 1ULL)          /* n has other prime factors */
        return (seq_t)1;

    /* n is a power of p.  Check ramified or inert condition */
    if (p == 2ULL || (p % 4ULL) == 3ULL)
        return (seq_t)p;
    else
        return (seq_t)1;
}
