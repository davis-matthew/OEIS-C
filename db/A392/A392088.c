/*
 * A392088  The unitary totient of the smallest cube divisible by n.
 * https://oeis.org/A392088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = A047994(A053149(n))
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1626, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Raise a seq_t base to an integer exponent exp. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res *= base;
    return res;
}

/* Unitary totient of the smallest cube divisible by n. */
seq_t a(idx_t n)
{
    seq_t result = 1;
    uint64_t m = (uint64_t)n;   /* work in native integer for factorisation */

    /* Trial division factorisation of n. */
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            /* Smallest cube exponent ≥ e: k = ceil(e/3) * 3 */
            idx_t k = ((e + 2) / 3) * 3;
            seq_t term = pow_seq((seq_t)p, k) - 1;
            result *= term;
        }
    }

    /* Remaining prime factor if any. */
    if (m > 1) {
        idx_t k = ((1 + 2) / 3) * 3;   /* equals 3 for exponent 1 */
        seq_t term = pow_seq((seq_t)m, k) - 1;
        result *= term;
    }

    return result;
}
