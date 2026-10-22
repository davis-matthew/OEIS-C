/*
 * A384780  Consecutive states of the linear congruential pseudo-random number
 *          generator 127107890972165*s mod 2^48 when started at 1.
 * https://oeis.org/A384780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 127107890972165 * a(n-1) mod 2^48, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A384780.
 * The sequence is defined by a linear congruential generator:
 *   a(1) = 1
 *   a(n) = (multiplier * a(n-1)) mod 2^48   for n > 1
 *
 * This uses a loop from 1 to n‑1, because the period is 2^46
 * and n is at most 10000 in the tests, so it is fast enough.
 * The implementation works for all supported integer sizes
 * (8/16/32/64/128 bits or arbitrary‑precision) as long as the
 * type can store at least 48 bits.  The modulo is performed
 * by masking the lower 48 bits after each multiplication. */
seq_t a(idx_t n)
{
    /* Constants: multiplier and modulo mask. */
    static const seq_t MULT = (seq_t)127107890972165ULL;      /* 127107890972165 */
    static const seq_t MASK = ((seq_t)1 << 48) - 1;          /* 2^48 – 1 */

    /* Offset starts at 1; behaviour for n == 0 is undefined but handled gracefully. */
    if (n <= 1) return 1;                                   /* a(1) = 1 */

    seq_t result = 1;                                       /* a(1) */
    for (idx_t i = 1; i < n; ++i) {
        result = (result * MULT) & MASK;                    /* mod 2^48 */
    }
    return result;
}
