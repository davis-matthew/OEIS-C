/*
 * A381887  a(n) = 1 if n != p^m*(p-1) for any prime p and any m >= 0,
 *          otherwise Product_{p in W} p, where W are the primes such that n =
 *          p^m*(p-1) for some m >= 0.
 * https://oeis.org/A381887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 4421, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 1 if there is no prime p such that n = p^m*(p-1) for some m≥0,
 * otherwise a(n) = product of all such primes p.
 * This is OEIS A381887.
 */

#include <stdint.h>
#include <stdbool.h>

/* Simple trial‑division primality test for 64‑bit values */
static bool is_prime_u64(uint64_t x) {
    if (x < 2) return false;
    if (x == 2) return true;
    if ((x & 1) == 0) return false;
    for (uint64_t d = 3; d * d <= x; d += 2) {
        if (x % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;            /* safety for out of offset */
    uint64_t N = (uint64_t)n;
    seq_t res = 1;

    /* Look for primes p up to n+1 */
    for (uint64_t p = 2; p <= N + 1; ++p) {
        if (!is_prime_u64(p)) continue;     /* skip composites */

        uint64_t pm = p - 1;
        if (N % pm != 0) continue;          /* p-1 must divide n */

        uint64_t q = N / pm;                /* n/(p-1) */
        if (q == 0) continue;               /* safety */

        uint64_t tmp = q;
        if (tmp == 1) {                     /* m = 0 case */
            res *= (seq_t)p;
            continue;
        }
        while (tmp > 1 && (tmp % p) == 0) {
            tmp /= p;
        }
        if (tmp == 1) {                     /* q is a power of p */
            res *= (seq_t)p;
        }
    }

    return res;
}
