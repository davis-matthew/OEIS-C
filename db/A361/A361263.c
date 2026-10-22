/*
 * A361263  Numbers of the form k*(k^5 +- 1)/2.
 * https://oeis.org/A361263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k*(k^5 ± 1)/2 where k = (n+1)/2 for odd n (minus sign) k
 *            = n/2 for even n (plus sign)
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 90, u64 n <= 3648,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the closed‑form expression for OEIS A361263. */

seq_t a(idx_t n)
{
    /* Determine k depending on parity of n. */
    seq_t k = (n & 1) ? (seq_t)((n + 1) / 2) : (seq_t)(n / 2);

    /* Compute k^5 in a wider integer to avoid overflow. */
    unsigned __int128 k128 = (unsigned __int128)k;
    unsigned __int128 k5   = k128 * k128 * k128 * k128 * k128;

    /* Form the numerator k*(k^5 ± 1). */
    unsigned __int128 prod;
    if (n & 1) {                /* odd n  →  minus 1 */
        prod = k128 * (k5 - 1);
    } else {                    /* even n →  plus 1 */
        prod = k128 * (k5 + 1);
    }

    /* Divide by 2.  The result is guaranteed to be integer. */
    prod /= 2;

    return (seq_t)prod;
}
