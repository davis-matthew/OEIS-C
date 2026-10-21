/*
 * A285957  {01->0}-transform of the Thue-Morse word A010060.
 * https://oeis.org/A285957
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

/* Transform: remove every 1 that follows a 0 in the Thue‑Morse word;
   the returned sequence is the infinite word obtained after this deletion. */

seq_t a(idx_t n)
{
    idx_t k = 1;          /* current position in Thue‑Morse (1‑based) */
    idx_t kept = 0;       /* number of bits kept so far */

    while (1) {
        /* Thue‑Morse bit at position k: 0 if popcount(k-1) even, 1 otherwise */
        seq_t bit = (seq_t)(__builtin_popcountll(k - 1) & 1);

        /* Skip a 1 that follows a 0 */
        if (bit == 1) {
            idx_t prev = k - 1;
            seq_t prev_bit = (seq_t)(__builtin_popcountll(prev - 1) & 1);
            if (prev_bit == 0) {
                k++;
                continue;
            }
        }

        kept++;
        if (kept == n)
            return bit;

        k++;
    }
}
