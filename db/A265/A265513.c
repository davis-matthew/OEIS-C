/*
 * A265513  a(n) = largest base-4 palindrome m <= n such that every base-4
 *          digit of m is <= the corresponding digit of n; m is written in
 *          base 10.
 * https://oeis.org/A265513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = largest base‑4 palindrome m ≤ n such that each digit of m
 *            * is ≤ the corresponding digit of n. If no such palindrome of *
 *            the same length exists (i.e., the required leading digit would
 *            be 0), * a(n) = 0
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial case where n is 0. */
    if (n == 0)
        return 0;

    /* Convert n to base‑4 digits (least significant first). */
    idx_t tmp = n;
    unsigned char d[64];      /* 4^32 > 2^64, enough for all idx_t values. */
    int len = 0;
    while (tmp) {
        d[len++] = (unsigned char)(tmp & 3);   /* tmp % 4  (shift right by 2) */
        tmp >>= 2;
    }

    /* Build the candidate palindrome digits. */
    unsigned char p[64];
    int i;
    for (i = 0; i < len; ++i) {
        unsigned char left  = d[i];
        unsigned char right = d[len - 1 - i];
        p[i] = (left < right) ? left : right;
    }

    /* Leading digit must be non‑zero; otherwise no valid palindrome. */
    if (p[len - 1] == 0)
        return 0;

    /* Reconstruct the decimal value from the palindrome digits. */
    seq_t res = 0;
    for (i = len - 1; i >= 0; --i)
        res = (res << 2) | (seq_t)p[i];

    return res;
}
