/*
 * A285972  {10->1}-transform of the Thue-Morse word A010060.
 * https://oeis.org/A285972
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

/* Replace each "10" in Thue–Morse by a single 1;
   output the n-th element (1‑based) of the resulting word. */
seq_t a(idx_t n)
{
    idx_t out = 0;       /* number of output bits produced */
    idx_t i   = 0;       /* current position in the input Thue–Morse word, 0‑based */

    while (out < n) {
        /* Parity of popcount(i) gives the Thue–Morse bit at position i */
        int b1 = __builtin_parityll(i);      /* bit at position i */
        int b2 = __builtin_parityll(i + 1);  /* bit at position i+1 */

        if (b1 == 1 && b2 == 0) {            /* found a "10" pattern */
            ++out;
            if (out == n)
                return (seq_t)1;            /* the replacement value */
            i += 2;                        /* skip both bits of the pattern */
        } else {                              /* output the single bit */
            ++out;
            if (out == n)
                return (seq_t)b1;
            ++i;                             /* move to the next input bit */
        }
    }

    /* The loop always returns before reaching this point. */
    return 0;
}
