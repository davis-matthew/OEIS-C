/*
 * A261491  a(n) = ceiling(2 + sqrt(8*n-4)).
 * https://oeis.org/A261491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(2 + sqrt(8*n - 4))
 * Range:     u8 n <= 32, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute the bit‑length of a seq_t value. */
static unsigned bitlen_seq(seq_t x)
{
    unsigned b = 0;
    while (x > 0) {
        x >>= 1;
        b++;
    }
    return b;
}

/* Integer floor square root of a seq_t value, using binary search. */
static seq_t isqrt_seq(seq_t x)
{
    if (x == 0)
        return 0;

    /* Upper bound: 1 << ceil(bitlen(x)/2) */
    unsigned hl = (bitlen_seq(x) + 1) / 2;
    seq_t low  = 0;
    seq_t high = (seq_t)1 << hl;   /* shift of seq_t is allowed   */

    while (low <= high) {
        seq_t mid  = (low + high) / 2;
        seq_t mid2 = mid * mid;
        if (mid2 <= x) {
            low = mid + 1;
        } else {
            if (mid == 0)
                break;
            high = mid - 1;
        }
    }
    return high;
}

/* Sequence value a(n) for n >= 1.  The function is stateless and works
   independently of any ordering of calls. */
seq_t a(idx_t n)
{
    /* Argument of the square root: 8*n - 4. */
    seq_t val = (seq_t)n * 8 - 4;

    seq_t t = isqrt_seq(val);                    /* floor(sqrt(val)) */
    seq_t result = 2 + t + ((t * t < val) ? 1 : 0);

    return result;
}
