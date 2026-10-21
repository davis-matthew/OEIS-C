/*
 * A266610  Decimal representation of the n-th iteration of the "Rule 41"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^((n mod 4)/2) for even n; a(n) = 2*4^n + 2 - 10*(n mod
 *            4) for odd n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t power_seq(seq_t base, idx_t exp)
/* Compute base^exp using binary exponentiation.  Suitable for both small
   and large exponents; for the 1 / 5‑power case the exponent is 0 or 1. */
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {                 /* even n */
        /* (n mod 4)/2 is 0 or 1 */
        idx_t k = (n % 4) / 2;
        return (k == 0) ? (seq_t)1 : (seq_t)5;
    } else {                            /* odd n */
        seq_t pow4_n = power_seq((seq_t)4, n);   /* 4^n */
        seq_t res = 2 * pow4_n;                 /* 2*4^n */
        res += 2;
        seq_t sub = (seq_t)(10 * (n % 4));       /* 10*(n mod 4) */
        res -= sub;
        return res;
    }
}
