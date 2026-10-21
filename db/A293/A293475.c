/*
 * A293475  a(n) = (3*n + 4)*Pochhammer(n, 4) / 4!.
 * https://oeis.org/A293475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n + 4) * n*(n+1)*(n+2)*(n+3) / 24, offset 0
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 126, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for n < 4: the product still works, but we can
       handle explicitly for clarity. */
    if (n == 0) return (seq_t)0;
    seq_t nums[4];
    nums[0] = (seq_t)n;
    nums[1] = (seq_t)(n + 1);
    nums[2] = (seq_t)(n + 2);
    nums[3] = (seq_t)(n + 3);

    /* Reduce the product by the denominator 24 before multiplying, to
       avoid overflow on 64‑bit seq_t. */
    seq_t denom = (seq_t)24;
    for (int i = 0; i < 4; ++i) {
        /* Euclidean gcd for seq_t */
        seq_t a = nums[i];
        seq_t b = denom;
        while (b != (seq_t)0) {
            seq_t t = a % b;
            a = b;
            b = t;
        }
        if (a != (seq_t)0) {
            nums[i] /= a;
            denom /= a;
        }
    }
    /* After reduction denom should be 1. */
    seq_t result = (seq_t)1;
    for (int i = 0; i < 4; ++i) {
        result *= nums[i];
    }

    /* Multiply by (3*n + 4) */
    seq_t factor = (seq_t)(3 * n + 4);
    result *= factor;

    return result;
}
