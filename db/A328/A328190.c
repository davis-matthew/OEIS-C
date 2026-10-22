/*
 * A328190  Lexicographically earliest infinite sequence of distinct positive
 *          integers such that the sequence and its first differences have no
 *          values in common.
 * https://oeis.org/A328190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For n ≥ 1 let t = ⌊(n-1)/4⌋. • If n ≡ 0 (mod 4) → a(n) = 5 t (t
 *            = n/4) • If n ≡ 1 (mod 4) → a(n) = 10 t + 1 (add 1 if t has an
 *            odd number of trailing 0‑bits) • If n ≡ 2 (mod 4) → a(n) = 5 t +
 *            3 (t = (n-2)/4) • If n ≡ 3 (mod 4) → a(n) = 10 t + 7 (t =
 *            (n-3)/4) All arithmetic is done in seq_t; trail...
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the four residue classes modulo 4 */
    switch ((int)(n & 3u))
    {
        case 0: /* n = 4t */
        {
            idx_t t = n >> 2;  /* n/4 */
            return (seq_t)(5ULL * t);
        }
        case 2: /* n = 4t + 2 */
        {
            idx_t t = (n - 2) >> 2;  /* (n-2)/4 */
            return (seq_t)(5ULL * t + 3);
        }
        case 3: /* n = 4t + 3 */
        {
            idx_t t = (n - 3) >> 2;  /* (n-3)/4 */
            return (seq_t)(10ULL * t + 7);
        }
        case 1: /* n = 4t + 1 */
        {
            idx_t t = (n - 1) >> 2;          /* (n-1)/4 */
            seq_t val = (seq_t)(10ULL * t + 1);
            if (t > 0)                       /* count trailing zeros of t */
            {
                unsigned tz = 0;
                idx_t tmp = t;
                while ((tmp & 1u) == 0) {
                    tmp >>= 1;
                    tz++;
                }
                if (tz & 1u)                  /* odd number of trailing zeros? */
                    val += 1;
            }
            return val;
        }
    }
    /* unreachable but keeps the compiler happy */
    return 0;
}
