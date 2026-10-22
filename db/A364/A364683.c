/*
 * A364683  a(n) is the least k such that 1 + 2^k + 3^k is divisible by n, or
 *          -1 if there is no such k.
 * https://oeis.org/A364683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = least k ≥ 0 with 1 + 2^k + 3^k ≡ 0 (mod n), or -1 if none
 * Range:     i8 n <= 178, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches k from 0 up to n.
   For each k we keep 2^k mod n and 3^k mod n in idx_t,
   so no overflow occurs even when seq_t is limited (e.g. 8‑bit). */

seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1, but guard the zero case */
        return -1;

    idx_t mod = (idx_t)n;   /* modulus for the entire calculation   */
    idx_t p2  = 1;          /* 2^k mod n */
    idx_t p3  = 1;          /* 3^k mod n */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* (1 + 2^k + 3^k) mod n */
        idx_t sum_mod = (1 + p2 + p3) % mod;
        if (sum_mod == 0)
            return (seq_t)k;

        /* advance to the next power */
        p2 = (p2 * 2) % mod;
        p3 = (p3 * 3) % mod;
    }
    return -1;   /* no such k exists */
}
