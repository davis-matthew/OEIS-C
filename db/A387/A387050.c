/*
 * A387050  Number of entries in the n-th row of Pascal's triangle not
 *          divisible by 16.
 * https://oeis.org/A387050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of binomial coefficients C(n,k) (0 ≤ k ≤ n)
 *            whose 2‑adic valuation is less than 4, i.e. C(n,k) is not
 *            divisible by 16. This is equivalent to counting the k for which
 *            the number of carries occurring in the binary addition of k and
 *            n−k is at most 3
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Number of bits needed to represent n.  For n == 0 this is 0.   */
    size_t num_bits = 0;
    idx_t temp = n;
    while (temp) {
        temp >>= 1;
        ++num_bits;
    }

    seq_t result = 0;                      /* accumulator */

    for (idx_t k = 0; k <= n; ++k) {
        idx_t nk = n - k;                  /* the complementary binomial index */

        int carries = 0;
        uint64_t carry = 0;                /* carry propagating to the next bit */

        /* Count carries during binary addition of k and nk = n−k.   */
        for (size_t i = 0; i < num_bits; ++i) {
            uint64_t bitk = (k >> i) & 1ULL;
            uint64_t bitnk = (nk >> i) & 1ULL;
            uint64_t sum  = bitk + bitnk + carry;

            if (sum >= 2ULL) {
                ++carries;
                if (carries >= 4)              /* too many carries → divisible by 16 */
                    break;
            }
            carry = sum >> 1;                 /* propagate carry (0 or 1) */
        }

        /* If carries < 4, then C(n,k) is not divisible by 16.      */
        if (carries < 4)
            ++result;
    }

    return result;
}
