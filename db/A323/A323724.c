/*
 * A323724  a(n) = n*(2*(n - 2)*n + (-1)^n + 3)/4.
 * https://oeis.org/A323724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*(n-2)*n + (-1)^n + 3)/4
 * Range:     u8 n <= 8, u16 n <= 51, u32 n <= 2047, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n = 0 or 1 gives 0 */
    if (n < 2) {
        return 0;
    }

    /* Work in the seq_t type to avoid intermediate
       signed arithmetic and keep the implementation
       compatible with both C and C++ builds. */
    seq_t N = (seq_t)n;                     /* n as seq_t */
    seq_t t = (seq_t)2 * (N - (seq_t)2) * N; /* 2*(n-2)*n */

    t += (seq_t)3;                          /* add +3 */

    /* Add or subtract 1 according to the parity of n.
       For n even we add 1, for n odd we subtract 1.
       t is always ≥ 3 for n ≥ 2, so the subtraction
       is safe in unsigned arithmetic. */
    if (n & 1) {
        t -= (seq_t)1;                      /* n odd */
    } else {
        t += (seq_t)1;                      /* n even */
    }

    /* Final product divided by 4 gives the sequence value */
    return N * t / 4;
}
