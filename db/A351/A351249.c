/*
 * A351249  a(n) = n^9 * Sum_{p|n, p prime} 1/p^9.
 * https://oeis.org/A351249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^9 * Sum_{p|n, p prime} 1/p^9 Equivalently, a(n) =
 *            Sum_{p|n} (n/p)^9 (for n >= 2, a(1)=0)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 23, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow9(seq_t x)
{
    seq_t r = 1;
    for (int i = 0; i < 9; ++i) r *= x;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    uint64_t nn = (uint64_t)n;      /* original n, fit in 64‑bit */
    uint64_t tmp = nn;              /* factorisation working copy */
    seq_t result = 0;

    /* Handle factor 2 */
    if ((tmp & 1) == 0) {
        uint64_t reduced = nn / 2;
        result += pow9((seq_t)reduced);
        while ((tmp & 1) == 0) tmp >>= 1;
    }

    /* Handle odd primes */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            uint64_t reduced = nn / p;
            result += pow9((seq_t)reduced);
            while (tmp % p == 0) tmp /= p;
        }
    }

    /* Remaining prime factor (if any) */
    if (tmp > 1) {
        uint64_t reduced = nn / tmp;
        result += pow9((seq_t)reduced);
    }

    return result;
}
