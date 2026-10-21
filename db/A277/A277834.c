/*
 * A277834  Number of '4' digits in the set of all numbers from 0 to
 *          A014824(n) = Sum_{i=1..n} i*10^(n-i) = (0, 1, 12, 123, 1234,
 *          12345, ...).
 * https://oeis.org/A277834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of digit '4' in decimal representations of all
 *            integers from 0 to M_n, where M_n = 123...n (concatenation of
 *            1..n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A277834. */
seq_t a(idx_t n)
{
    /* Construct M_n = 1,2,3,...,n as a big integer. */
    seq_t M = 0;
    for (idx_t d = 1; d <= n; ++d) {
        M = M * (seq_t)10 + d;
    }

    /* Count digit '4's using standard digit‑position analysis. */
    seq_t result = 0;
    seq_t temp   = M;     /* Remaining part of the number to process. */
    seq_t low    = 0;     /* Value contributed by lower (less significant) digits. */
    seq_t weight = 1;     /* 10^position of the current digit (starting from 1). */

    while (temp != 0) {
        seq_t cur  = temp % (seq_t)10;   /* Current least significant digit. */
        seq_t high = temp / (seq_t)10;   /* Digits left of the current one. */

        if (cur > (seq_t)4) {
            /* All combinations where current digit is 4: (high+1) * weight. */
            result += (high + (seq_t)1) * weight;
        } else if (cur == (seq_t)4) {
            /* Current digit is 4: high * weight + low + 1. */
            result += high * weight + low + (seq_t)1;
        } else {
            /* Current digit is <4: high * weight. */
            result += high * weight;
        }

        /* Update low and move to next higher digit. */
        low    += cur * weight;
        weight *= (seq_t)10;
        temp    = high;
    }

    return result;
}
