/*
 * A319843  A self-describing sequence in English and in base 2 (see the
 *          Comments section for the explanation).
 * https://oeis.org/A319843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, word
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 298, u32 n <= 298, u64 n <= 298,
 *            u128 n <= 298, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: 
 * The sequence is the infinite fixed point of the morphism
 *   1 → 1 0 1 1 0 1 0 1 1   (the binary pattern of the word "consonant")
 *   0 → 1 0 1 0 1           (the binary pattern of the word "vowel")
 * starting from the single symbol 1.
 * a(n) is the n‑th symbol (1–indexed) of that infinite word.
 * (The first 24 terms are   1,0,1,1,0,1,0,1,1,1,0,1,0,1,1,0,1,1,0,1,0,1,1,1.)
 */

seq_t a(idx_t n)
{
    /* patterns for the words "consonant" (9 letters) and "vowel" (5 letters) */
    static const seq_t cons[9] = {1, 0, 1, 1, 0, 1, 0, 1, 1};
    static const seq_t vow[5]  = {1, 0, 1, 0, 1};

    if (n == 0)
        return 0;               /* safety; the sequence is 1‑based */

    /* --- pre‑compute expansion lengths up to the required depth --- */
    seq_t len[2][64];           /* len[0][d] = length of symbol 0 after d steps,
                                 * len[1][d] = length of symbol 1 after d steps      */
    len[0][0] = len[1][0] = 1;

    idx_t depth = 0;
    while (len[1][depth] < (seq_t)n) {
        ++depth;
        seq_t l1 = 0, l0 = 0;
        for (int i = 0; i < 9; ++i)        /* from symbol 1 */
            l1 += len[(int)cons[i]][depth - 1];
        for (int i = 0; i < 5; ++i)        /* from symbol 0 */
            l0 += len[(int)vow[i]][depth - 1];
        len[1][depth] = l1;
        len[0][depth] = l0;
    }

    /* --- descend the morphic tree to find the n‑th symbol --- */
    seq_t idx = (seq_t)n - 1;   /* 0‑based index inside the final word */
    seq_t symbol = 1;           /* start from the seed symbol 1 */

    while (depth > 0) {
        const seq_t *pattern;
        int plen;
        if (symbol == 1) {
            pattern = cons;
            plen    = 9;
        } else {
            pattern = vow;
            plen    = 5;
        }

        /* find the child that contains the current index */
        for (int i = 0; i < plen; ++i) {
            seq_t sublen = len[(int)pattern[i]][depth - 1];
            if (idx < sublen) {
                symbol = pattern[i];
                depth--;
                break;
            }
            idx -= sublen;
        }
    }

    return symbol;
}
