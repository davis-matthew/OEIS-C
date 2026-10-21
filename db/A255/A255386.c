/*
 * A255386  Number of binary words of length n with exactly one occurrence of
 *          subword 010 and exactly one occurrence of subword 101.
 * https://oeis.org/A255386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 40, u64 n <= 83, u128 n <= 172,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A255386: Number of binary words of length n with exactly one
   occurrence of subword 010 and exactly one occurrence of subword 101. */
seq_t a(idx_t n)
{
    /* For lengths < 4 no word can contain both patterns */
    if (n < 4)
        return 0;

    /* cur[occ010][occ101][state]  state encodes last two bits:
       0 = 00, 1 = 01, 2 = 10, 3 = 11 */
    static seq_t cur[2][2][4];
    static seq_t nxt[2][2][4];
    int i, j, s, t;

    /* initialise current layer with all 2‑bit strings */
    for (i = 0; i < 2; ++i)
        for (j = 0; j < 2; ++j)
            for (s = 0; s < 4; ++s)
                cur[i][j][s] = 0;
    for (s = 0; s < 4; ++s)
        cur[0][0][s] = 1;

    /* extend strings up to length n */
    for (idx_t len = 2; len < n; ++len)
    {
        /* zero next layer */
        for (i = 0; i < 2; ++i)
            for (j = 0; j < 2; ++j)
                for (s = 0; s < 4; ++s)
                    nxt[i][j][s] = 0;

        for (i = 0; i < 2; ++i)          /* occ010  */
            for (j = 0; j < 2; ++j)      /* occ101  */
                for (s = 0; s < 4; ++s)
                {
                    seq_t cur_val = cur[i][j][s];
                    if (!cur_val)
                        continue;
                    int a = (s >> 1) & 1;   /* first bit of the pair   */
                    int b = s & 1;          /* second bit of the pair  */
                    for (t = 0; t < 2; ++t) /* new bit to append      */
                    {
                        int ni = i, nj = j;
                        /* would appending t create a new 010? */
                        if (a == 0 && b == 1 && t == 0) {
                            if (i == 1)      /* would exceed one 010 */
                                continue;
                            ni = 1;
                        }
                        /* would it create a new 101? */
                        if (a == 1 && b == 0 && t == 1) {
                            if (j == 1)      /* would exceed one 101 */
                                continue;
                            nj = 1;
                        }
                        int ns = (b << 1) | t;     /* new state    */
                        nxt[ni][nj][ns] += cur_val;
                    }
                }

        /* swap cur and nxt */
        for (i = 0; i < 2; ++i)
            for (j = 0; j < 2; ++j)
                for (s = 0; s < 4; ++s)
                    cur[i][j][s] = nxt[i][j][s];
    }

    /* sum over all states with exactly one 010 and one 101 */
    seq_t result = 0;
    for (s = 0; s < 4; ++s)
        result += cur[1][1][s];
    return result;
}
