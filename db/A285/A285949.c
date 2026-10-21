/*
 * A285949  {0->01, 1->0}-transform of the Thue-Morse word A010060.
 * https://oeis.org/A285949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th entry of the infinite word obtained from * the
 *            Thue‑Morse sequence by mapping each 0 to "01" and each 1 to "0"
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t idx = 0;        /* index into Thue‑Morse word, 0‑based */
    idx_t outPos = 0;     /* number of output bits produced so far */

    while (1) {
        /* Thue‑Morse bit: parity of popcount(idx) */
        int t = (__builtin_popcountll((unsigned long long)idx) & 1);

        if (t == 0) {            /* maps to "01" */
            if (outPos + 1 == n) return (seq_t)0;
            if (outPos + 2 == n) return (seq_t)1;
            outPos += 2;
        } else {                 /* maps to "0" */
            if (outPos + 1 == n) return (seq_t)0;
            outPos += 1;
        }
        idx++;
    }
}
