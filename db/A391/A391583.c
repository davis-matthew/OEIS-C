/*
 * A391583  a(n) is the value of A280500(n*k, k) for the third odd number k
 *          for which this value is not zero, where A280500 implements the
 *          carryless base-2 division, and returns 0 if the division leaves
 *          nonzero remainder.
 * https://oeis.org/A391583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = second non‑zero quotient of carryless polynomial division
 *            (n*k)/k over odd k > 1, where k is the smallest odd integer > 1
 *            that yields an exact carryless division
 * Range:     u8 n <= 21, u16 n <= 1821, u32 n <= 9452, u64 n <= 9452,
 *            u128 n <= 9452, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Compute bit length (position of most significant 1 bit) of x, 0 for x==0 */
static inline int bitlen_seq_t(seq_t x)
{
    int len = 0;
    while (x) {
        x >>= 1;
        ++len;
    }
    return len;
}

/* Carryless base‑2 division: quotient if exact, otherwise 0 */
static inline seq_t carryless_div(seq_t dividend, seq_t divisor)
{
    if (divisor == 0) return 0;
    seq_t q = 0;
    while (dividend != 0) {
        int deg_dividend = bitlen_seq_t(dividend) - 1;
        int deg_divisor  = bitlen_seq_t(divisor)  - 1;
        if (deg_dividend < deg_divisor) break;
        idx_t shift = (idx_t)(deg_dividend - deg_divisor);
        q ^= ((seq_t)1 << shift);
        dividend ^= (divisor << shift);
    }
    return (dividend == 0) ? q : (seq_t)0;
}

seq_t a(idx_t n)
{
    idx_t k = 3;          /* start with the smallest odd > 1 */
    int   count = 0;      /* number of odd k with non‑zero quotient */

    while (1) {
        seq_t prod = (seq_t)n * (seq_t)k;   /* product n*k in seq_t */
        seq_t q    = carryless_div(prod, (seq_t)k);
        if (q != 0) {
            ++count;
            if (count == 2) return q;
        }
        k += 2;          /* next odd k */
    }
    /* unreachable, keep compiler happy */
    return 0;
}
