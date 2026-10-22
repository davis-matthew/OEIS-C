/*
 * A333340  a(n) is the smallest positive number k such that n divides 4^k -
 *          k.
 * https://oeis.org/A333340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimum k ≥ 1 such that n divides (4^k – k) (direct
 *            search)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)        /* undefined for n = 0, but return 0 safely */
        return 0;
    if (n == 1)
        return 1;

    /* If n is a power of two, the answer is n itself. */
    if ((n & (n - 1)) == 0)
        return (seq_t)n;

    idx_t pow_mod = 4 % n;   /* 4^k mod n */
    idx_t k_mod   = 1 % n;   /* k mod n */
    seq_t k_seq   = 1;       /* full k value as seq_t */

    for (;;) {
        if (pow_mod == k_mod)
            return k_seq;

        /* advance to next k */
        k_seq++;
        k_mod++;
        if (k_mod == n)
            k_mod = 0;

        /* multiply pow_mod by 4 mod n, using 128‑bit arithmetic to avoid overflow */
        pow_mod = (unsigned __int128)pow_mod * 4 % n;
    }
}
