/*
 * A286493  {1011->0}-transform of the Thue-Morse word A010060.
 * https://oeis.org/A286493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Replaces each 1011 by 0 in the Thue–Morse word A010060. */
seq_t a(idx_t n)
{
    idx_t pos = 0;      /* index in the original Thue–Morse word (0‑based) */
    idx_t out = 0;      /* number of output bits produced so far */

    while (1) {
        /* Does the word starting at pos contain 1011? */
        if ((__builtin_parityll(pos) == 1) &&
            (__builtin_parityll(pos + 1) == 0) &&
            (__builtin_parityll(pos + 2) == 1) &&
            (__builtin_parityll(pos + 3) == 1)) {
            out++;
            if (out == n)
                return (seq_t)0;   /* the replacement for the pattern */
            pos += 4;              /* skip the four original bits */
            continue;
        }

        /* Output the current bit of the Thue–Morse word. */
        out++;
        if (out == n)
            return (seq_t)__builtin_parityll(pos);
        pos++;
    }
}
