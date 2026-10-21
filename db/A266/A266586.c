/*
 * A266586  The least nonnegative integer N such that n*N has the same digits
 *          as n and N together, not counting repetitions.
 * https://oeis.org/A266586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the least nonnegative integer N such that the * set of
 *            digits of n*N equals the set of digits of the concatenation * of
 *            n and N (duplicates ignored). For any n having the digit 1, *
 *            a(n)=1
 * Range:     u8 n <= 1, u16 n <= 36, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* compute bit mask of distinct decimal digits of x (bits 0..9) */
static unsigned digit_mask(seq_t x)
{
    unsigned mask = 0;
    if (x == 0)
        return 1u;           /* only digit 0 */
    while (x > 0) {
        unsigned d = (unsigned)(x % 10);
        mask |= 1u << d;
        x /= 10;
    }
    return mask;
}

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    unsigned mask_n = digit_mask(n_seq);

    /* Rule: if n already contains 1, answer is 1 */
    if (mask_n & (1u << 1))
        return (seq_t)1;

    /* Search for the minimal N>=1 */
    for (idx_t N = 1; ; ++N) {
        seq_t N_seq  = (seq_t)N;
        unsigned maskN  = digit_mask(N_seq);
        unsigned unionMask = mask_n | maskN;

        seq_t prod   = n_seq * N_seq;
        unsigned maskP = digit_mask(prod);

        if (maskP == unionMask)
            return N_seq;
    }
}
