/*
 * A367699  The smallest divisor d of n such that n/d is an exponentially evil
 *          number (A262675).
 * https://oeis.org/A367699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult, base
 * Formula:   a(n) = product over p^e || n of p^(e - s(e)) * where s(e) =
 *            largest evil ≤ e (evil numbers are those * with an even number
 *            of 1‑bits). This equals n divided * by the greatest
 *            exponentially evil divisor of n
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/*--- helper: popcount for small integers --------------------------------*/
static int popcount_u64(uint64_t x) {
    int c = 0;
    while (x) { c += (int)(x & 1ULL); x >>= 1; }
    return c;
}

/*--- helper: largest evil ≤ e, cached up to 64 ---------------------------*/
static int largest_evil_leq(int e) {
    static int cache[65];
    static int initialized = 0;
    if (!initialized) {
        initialized = 1;
        for (int i = 0; i <= 64; ++i) {
            int pc = popcount_u64((uint64_t)i);
            cache[i] = (pc & 1) == 0 ? i : (i > 0 ? cache[i - 1] : -1);
        }
    }
    return cache[e];
}

/*--- main sequence function -----------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of OEIS domain, but harmless */
    if (n == 1) return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t tmp = n;

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p != 0) continue;

        int e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }

        int s = largest_evil_leq(e);
        int exp = e - s;
        for (int i = 0; i < exp; ++i)
            result *= (seq_t)p;
    }

    /* Remaining prime factor > sqrt(n) */
    if (tmp > 1) {
        /* For exponent e = 1, s(1) = 0 (since 1 is odd‑popcount). */
        result *= (seq_t)tmp;
    }

    return result;
}
