/*
 * A382151  Primes p such that some elliptic curve over Q admits a Q-rational
 *          p-isogeny.
 * https://oeis.org/A382151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A382151: Prime numbers p such that some elliptic curve over Q
 * admits a Q-rational p-isogeny. This set is known to be finite:
 * 2, 3, 5, 7, 11, 13, 17, 19, 37, 43, 67, 163.  No further terms
 * occur.  The function returns a(1) .. a(12) and returns 0 for any
 * n > 12 or n < 1.
 */
seq_t a(idx_t n)
{
    static const seq_t primes[12] = {
        (seq_t)2,
        (seq_t)3,
        (seq_t)5,
        (seq_t)7,
        (seq_t)11,
        (seq_t)13,
        (seq_t)17,
        (seq_t)19,
        (seq_t)37,
        (seq_t)43,
        (seq_t)67,
        (seq_t)163
    };

    if (n == 0) return (seq_t)0;
    if (n <= 12) return primes[n - 1];
    return (seq_t)0;
}
