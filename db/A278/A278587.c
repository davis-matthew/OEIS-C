/*
 * A278587  Value of the Catch-Up game [1,...n] for first player (1 = win, -1
 *          = loss, 0 = draw).
 * https://oeis.org/A278587
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign, more
 * Formula:   for n < 13 the sequence values are fixed. For n ≥ 13, a(n) = 1
 *            if n ≡ 1 (mod 4); a(n) = –1 if n ≡ 2 (mod 4); otherwise a(n) = 0
 * Range:     i8 n <= 20, i16 n <= 20, i32 n <= 20, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the Catch‑Up game value for first player. */
seq_t a(idx_t n)
{
    /* Handle indices below the allowed offset safely. */
    if (n < 3) return 0;

    /* Special cases for n = 3 … 12. */
    if (n < 13)
    {
        /* index 0 → n = 3, index 9 → n = 12 */
        static const seq_t tbl[10] = {
            0,  /* a(3)  */
            0,  /* a(4)  */
            1,  /* a(5)  */
            1,  /* a(6)  */
            0,  /* a(7)  */
            0,  /* a(8)  */
           -1,  /* a(9)  */
           -1,  /* a(10) */
            0,  /* a(11) */
            0   /* a(12) */
        };
        return tbl[n - 3];
    }

    /* For n ≥ 13 use the simple mod‑4 rule. */
    switch ((unsigned)(n % 4))
    {
        case 1:  return 1;   /* n ≡ 1 (mod 4) */
        case 2:  return -1;  /* n ≡ 2 (mod 4) */
        default: return 0;   /* n ≡ 0 or 3 (mod 4) */
    }
}
