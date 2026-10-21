/*
 * A271624  a(n) = 2*n^2 - 4*n + 4.
 * https://oeis.org/A271624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2 - 4*n + 4
 * Range:     u8 n <= 12, u16 n <= 182, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = n;
    /* Compute 2*N*N + 4 first to avoid unsigned underflow,
       then subtract 4*N. For all n >= 0 the intermediate
       result is large enough that the subtraction never underflows. */
    seq_t result = 2 * N * N + 4;
    result -= 4 * N;
    return result;
}
