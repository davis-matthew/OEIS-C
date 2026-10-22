/*
 * A352378  Irregular triangle read by rows: T(n,k) is the (n-th)-to-last
 *          digit of 2^p such that p == k + A123384(n-1) (mod A005054(n)); k
 *          >= 0.
 * https://oeis.org/A352378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, base
 * Formula:   a(n) = digit at position r of 2^p where r is the row such that
 *            5^(r-1) – 1 < n ≤ 5^r – 1, p = (3 · (r‑1) + 1) + (n – (5^(r‑1) –
 *            1) – 1), and the digit is the r‑th‑to‑last decimal digit of 2^p
 * Range:     u8 n <= 25, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t mod_pow_seq(seq_t base, idx_t exp, seq_t mod)
{
    seq_t result = 1 % mod;
    seq_t b = base % mod;
    while (exp) {
        if (exp & 1U)
            result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Determine the row r and position k within that row. */
    unsigned __int128 pow5 = 1;          /* 5^(r-1) */
    idx_t          r     = 1;
    idx_t          cum   = 0;           /* cumulative length up to row r-1 */

    while (1) {
        unsigned __int128 L128 = 4 * pow5;   /* period length for row r */
        idx_t          L   = (idx_t)L128;
        if (cum + L >= n) {
            /* r is found */
            break;
        }
        cum += L;
        pow5 *= 5;
        ++r;
    }

    /* Position k within the row (0‑based). */
    idx_t k = n - cum - 1U;

    /* Offset A123384(r‑1) = 3 · (r‑1) + 1. */
    idx_t offset = 3 * (r - 1) + 1;
    /* Exponent of 2. */
    idx_t p = offset + k;

    /* Compute 10^r and 10^(r‑1) for digit extraction. */
    seq_t pow10_prev = 1;
    for (idx_t i = 1; i < r; ++i)
        pow10_prev *= 10;
    seq_t pow10 = pow10_prev * 10;

    /* 2^p modulo 10^r. */
    seq_t q = mod_pow_seq((seq_t)2, (idx_t)p, pow10);
    /* The r‑th‑to‑last digit of 2^p. */
    seq_t digit = q / pow10_prev;

    return digit;
}
