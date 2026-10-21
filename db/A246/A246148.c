/*
 * A246148  Number of inequivalent n X 3 binary matrices, where equivalence
 *          means permutations of rows or columns or the symbol set.
 * https://oeis.org/A246148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of inequivalent n×3 binary matrices (up to
 *            row/column permutations and symbol complement). Computation uses
 *            Burnside's lemma on the action of G = S_3 × C_2 on the 8 binary
 *            vectors
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 109, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const int _perms[6][3] = {
    {0,1,2}, /* identity */
    {1,0,2}, /* swap 0↔1 */
    {2,1,0}, /* swap 0↔2 */
    {0,2,1}, /* swap 1↔2 */
    {1,2,0}, /* cycle 0→1→2 */
    {2,0,1}  /* cycle 0→2→1 */
};

static unsigned _cycles[12][8];
static unsigned _cycle_len[12];
static int _initialized = 0;

static void _init_cycles(void)
{
    int idx = 0;
    for (int p = 0; p < 6; ++p) {
        for (int c = 0; c < 2; ++c) {
            unsigned map[8];
            /* build mapping */
            for (int a = 0; a < 8; ++a) {
                unsigned perm = 0;
                for (int j = 0; j < 3; ++j)
                    if ((a >> _perms[p][j]) & 1u)
                        perm |= 1u << j;
                if (c)
                    perm ^= 7u; /* complement */
                map[a] = perm;
            }
            /* compute cycles */
            int visited[8] = {0};
            int clen = 0;
            for (int i = 0; i < 8; ++i) {
                if (!visited[i]) {
                    int cur = i, len = 0;
                    do {
                        visited[cur] = 1;
                        cur = map[cur];
                        ++len;
                    } while (cur != i);
                    _cycles[idx][clen++] = (unsigned)len;
                }
            }
            _cycle_len[idx] = (unsigned)clen;
            ++idx;
        }
    }
    _initialized = 1;
}

seq_t a(idx_t n)
{
    if (!_initialized)
        _init_cycles();

    size_t N = (size_t)n;
    seq_t *dp = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!dp) return 0; /* allocation failure – unlikely */

    seq_t sum = 0;
    for (int g = 0; g < 12; ++g) {
        /* reset dp */
        for (size_t i = 0; i <= N; ++i)
            dp[i] = 0;
        dp[0] = 1;

        unsigned *len = _cycles[g];
        unsigned clen = _cycle_len[g];
        for (unsigned ci = 0; ci < clen; ++ci) {
            unsigned l = len[ci];
            for (size_t t = l; t <= N; ++t)
                dp[t] += dp[t - l];
        }
        sum += dp[N];
    }
    free(dp);
    return sum / 12;
}
