/*
 * A359244  Number of divisors of 5*n-4 of form 5*k+2.
 * https://oeis.org/A359244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of (5*n-4) that are congruent to 2
 *            (mod 5)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Small structure to hold prime factor and its exponent. */
typedef struct {
    uint64_t p;
    int exp;
} Factor;

/* Recursive enumeration of all divisors of n from its prime factors. */
static void _enumerate(const Factor *fac, int fcnt, int idx,
                       uint64_t cur, uint64_t *cnt)
{
    if (idx == fcnt) {
        /* Count the divisor only if it is ≡ 2 (mod 5). */
        if (cur % 5 == 2)
            (*cnt)++;
        return;
    }
    uint64_t val = cur;
    for (int e = 0; e <= fac[idx].exp; ++e) {
        _enumerate(fac, fcnt, idx + 1, val, cnt);
        val *= fac[idx].p;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Compute m = 5*n - 4.  n is small enough that this fits in 64 bits. */
    uint64_t m = 5ULL * n - 4ULL;

    /* Factorise m.  At most 20 distinct prime factors for numbers in this range. */
    Factor fac[20];
    int fac_cnt = 0;
    uint64_t t = m;
    for (uint64_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            fac[fac_cnt++] = (Factor){p, e};
        }
    }
    if (t > 1) {
        fac[fac_cnt++] = (Factor){t, 1};
    }

    /* Enumerate all divisors and count those ≡ 2 (mod 5). */
    uint64_t count = 0;
    _enumerate(fac, fac_cnt, 0, 1ULL, &count);

    return (seq_t)count;
}
