/*
 * A357531  Final value obtained by traveling clockwise around a circular
 *          array with positions numbered clockwise from 1 to n. Each move
 *          consists of traveling clockwise k places, where k is the position
 *          at the beginning of the move. The first move begins at position 1.
 *          a(n) is the position at the end of the...
 * https://oeis.org/A357531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((2^n - 1) mod n) + 1
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not in OEIS range, but safe */
    seq_t N = (seq_t)n;            /* modulo base */

    /* compute 2^n mod n with binary exponentiation */
    seq_t result = 1;              /* accumulates 2^k mod n */
    seq_t base   = 2;              /* 2^(1<<i) mod n */
    idx_t exp   = n;

    while (exp) {
        if (exp & 1) {
            unsigned long long tmp = (unsigned long long)result * (unsigned long long)base;
            result = (seq_t)(tmp % N);
        }
        exp >>= 1;
        if (exp) { /* avoid last square when exp becomes 0 */
            unsigned long long tmp = (unsigned long long)base * (unsigned long long)base;
            base = (seq_t)(tmp % N);
        }
    }

    /* Now result = 2^n mod n. Compute (result - 1) mod n and add 1. */
    seq_t r_minus1 = (result + N - 1) % N;
    return r_minus1 + 1;
}
