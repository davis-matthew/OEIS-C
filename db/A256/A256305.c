/*
 * A256305  Apply the transformation 0 -> 1 -> 2 -> 3 -> 4 -> 0 to the digits
 *          of n written in base 5; do not convert back to base 10.
 * https://oeis.org/A256305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = decimal number formed from the base‑5 digits of n, each
 *            digit being increased by 1 modulo 5 (0→1, 1→2, 2→3, 3→4, 4→0)
 * Range:     u8 n <= 49, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t             res   = 0;
    idx_t             t     = n;
    /* 5‑ary representation needs at most 27 digits for 64‑bit n.
       32 is plenty for any variant of seq_t. */
    uint8_t           digits[32];
    int               num   = 0;

    /* Extract digits in least‑significant to most‑significant order. */
    do
    {
        idx_t d = t % 5;
        d = (d + 1) % 5;                  /* apply the transformation */
        digits[num++] = (uint8_t)d;
        t /= 5;
    } while (t);

    /* Construct the decimal number from most‑significant digit to least. */
    for (int i = num - 1; i >= 0; --i)
        res = res * 10 + digits[i];

    return res;
}
