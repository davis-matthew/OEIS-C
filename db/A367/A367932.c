/*
 * A367932  a(n) is the smallest number k such that k*n is an exponentially
 *          evil number (A262675).
 * https://oeis.org/A367932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult, base
 * Formula:   a(n) = ∏_{p^e‖n} p^{s(e)} where s(e) = min{k ≥ e, k evil} – e,
 *            and evil(k) ⇔ the binary representation of k contains an even
 *            number of 1‑bits. This follows from the definition of A367932
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static bool is_evil(unsigned long long x)
{
    /* Popcount parity: 0 = even, 1 = odd */
    unsigned int parity = 0;
    while (x) {
        parity ^= 1;          /* flip parity for each set bit */
        x &= x - 1;           /* clear lowest set bit */
    }
    return parity == 0;
}

static unsigned long long next_even_popcount(unsigned long long e)
{
    while (!is_evil(e))
        ++e;
    return e;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t temp = n;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            unsigned long long next = next_even_popcount(exp);
            idx_t add = next - exp;
            for (idx_t i = 0; i < add; ++i)
                result *= (seq_t)p;
        }
    }

    if (temp > 1) {
        /* Remaining factor is prime: exponent 1 */
        unsigned long long next = next_even_popcount(1);
        idx_t add = next - 1;          /* add = 2 for prime factors */
        for (idx_t i = 0; i < add; ++i)
            result *= (seq_t)temp;
    }

    return result;
}
