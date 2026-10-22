/*
 * A346789  a(n) is the smallest integer m, such that for every sufficiently
 *          large integer k, A165370(729*k+n) can be written as m followed by
 *          zero or more 9's.
 * https://oeis.org/A346789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   a(n) = A165370(5103 + n) with all trailing 9's removed
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 728, u128 n <= 728,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A165370(n) is the smallest integer whose sum of cubes of digits equals n */

#include <stddef.h>

#define MAX_S 16000            /* enough for 729*7 + 10000 + safety margin */

static seq_t best_val[MAX_S + 1];
static unsigned char best_len[MAX_S + 1];
static seq_t pow10[64];
static int initialized = 0;

static void init_tables(void)
{
    /* powers of 10 */
    pow10[0] = (seq_t)1;
    for (int i = 1; i < 64; ++i)
        pow10[i] = pow10[i - 1] * (seq_t)10;

    best_len[0] = 0;
    best_val[0] = (seq_t)0;

    for (int s = 1; s <= MAX_S; ++s) {
        best_len[s] = 0;      /* 0 = not yet found */
        for (int d = 1; d <= 9; ++d) {
            int cube = d * d * d;
            if (cube > s) continue;
            int rest = s - cube;
            unsigned char len_rest = best_len[rest];
            if (len_rest == 0 && rest > 0) continue; /* unreachable remainder */
            unsigned char cand_len = len_rest + 1;
            seq_t cand_val = (seq_t)d * pow10[(int)len_rest] + best_val[rest];
            if (best_len[s] == 0 ||
                cand_len < best_len[s] ||
                (cand_len == best_len[s] && cand_val < best_val[s])) {
                best_len[s] = cand_len;
                best_val[s] = cand_val;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        initialized = 1;
        init_tables();
    }

    /* required sum for A165370 */
    idx_t s_idx = (idx_t)5103 + n;        /* 729*7 = 5103 */
    if (s_idx > (idx_t)MAX_S)            /* out of pre‑computed range */
        return (seq_t)0;

    seq_t m = best_val[(int)s_idx];

    /* strip all trailing 9's */
    while (m % (seq_t)10 == (seq_t)9)
        m = m / (seq_t)10;

    return m;
}
