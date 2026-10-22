/*
 * A350229  a(n) is the sum of n and the balanced ternary digits in n.
 * https://oeis.org/A350229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n + Σ d_i, where d_i are the balanced ternary digits of n
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t x = n;           /* working copy of n */
    int64_t sum = 0;        /* signed sum of balanced ternary digits */

    while (x > 0) {
        idx_t rem = x % 3;   /* remainder in {0,1,2} */
        x /= 3;
        if (rem == 2) {      /* digit 2 -> -1 with a carry */
            rem = -1;
            x += 1;
        }
        sum += (int64_t)rem; /* accumulate signed digit */
    }

    /* Combine original n and the signed digit sum.  The result is guaranteed
       to be non‑negative.  Use signed arithmetic to avoid unsigned wrap.*
       Convert back to seq_t at the end. */
    seq_t result;
    if (sum >= 0)
        result = (seq_t)n + (seq_t)sum;
    else
        result = (seq_t)n - (seq_t)(-sum);

    return result;
}
