/*
 * A392781  Length of the longest reduced word in the cyclic group C_n.
 * https://oeis.org/A392781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ a(p^e) over prime factorization of n where a(2^e) = (e
 *            + e mod 2)/2 and a(p^e) = e*(p-1)/2 for odd prime p
 * Range:     u8 n <= 520, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the value of a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n=0, return 0 */
    seq_t res = 0;
    idx_t m = n;

    /* Handle factor 2 separately */
    if ((m & 1ULL) == 0) {
        idx_t e = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            e++;
        }
        res += ((seq_t)e + (seq_t)(e & 1)) / 2;
    }

    /* Now factor odd part */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            /* p is odd prime here */
            res += (seq_t)e * (seq_t)(p - 1) / 2;
        }
    }

    /* If what remains is a prime > 1 */
    if (m > 1) {
        /* m is odd prime */
        res += (seq_t)(m - 1) / 2;
    }

    return res;
}
