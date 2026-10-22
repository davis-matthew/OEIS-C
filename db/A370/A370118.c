/*
 * A370118  a(n) = 1 if the arithmetic derivative of n is a multiple of 9,
 *          otherwise 0.
 * https://oeis.org/A370118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4049, u16 n <= 8547, u32 n <= 8547, u64 n <= 8547,
 *            u128 n <= 8547, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 1 if the arithmetic derivative d(n) is a multiple of 9, else 0 */
/* d(n) = Σ (n/p_i)*e_i  over all prime factors p_i of n with exponent e_i */

seq_t a(idx_t n)
{
    /* Special cases: d(0)=d(1)=0, which is a multiple of 9. */
    if (n == 0 || n == 1)
        return (seq_t)1;

    idx_t rem = n;
    int mod9 = 0;                      /* d(n) modulo 9 */

    for (idx_t p = 2; p <= rem / p; ++p) {
        if (rem % p != 0)
            continue;

        int exp = 0;
        while (rem % p == 0) {
            rem /= p;
            ++exp;
        }

        /* term = (n / p) * e  (mod 9) */
        int term = (int)((seq_t)(n / p) % 9) * exp;
        mod9 += term;
        mod9 %= 9;
    }

    /* Remaining factor > 1 is a prime. */
    if (rem > 1) {
        int term = (int)((seq_t)(n / rem) % 9); /* exp = 1 */
        mod9 += term;
        mod9 %= 9;
    }

    return (mod9 % 9 == 0) ? (seq_t)1 : (seq_t)0;
}
