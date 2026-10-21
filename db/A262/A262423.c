/*
 * A262423  Number of (4+1)X(n+1) 0..1 arrays with each row divisible by 3 and
 *          column not divisible by 3, read as a binary number with top and
 *          left being the most significant bits.
 * https://oeis.org/A262423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: dynamic programming over columns.
   Each 5×(n+1) binary array is built column by column.
   Allowed column patterns are the 5‑bit numbers whose binary value
   (with the leftmost bit as most significant) is NOT divisible by 3.
   For each column we add the column pattern times 2^{position} (mod 3)
   to the residue vector of the five rows.
   A row is divisible by 3 iff its residue vector component is 0.
   We count the number of ways to reach the all‑zero residue vector
   after processing all n+1 columns.  This DP uses 243 states
   (3^5) and at most 31 admissible column patterns; the transition
   table is pre‑computed once.  The function is memoised but
   works for calls in any order and for all n ≥ 1. */

#include <stdbool.h>

/* Helper: popcount for 32‑bit masks. */
static inline int popcnt32(unsigned int x) { return __builtin_popcount(x); }

/* Constants. */
#define STATE_CNT 243     /* 3^5 */
#define MAX_VEC_CNT 32    /* maximum number of admissible column vectors */

/* Pre‑computed data. */
static bool _precomp_done = false;
static int _trans[2][STATE_CNT][MAX_VEC_CNT]; /* weight 0 (1), weight 1 (2) */
static int  _state_bits[STATE_CNT][5];
static int  _vec[31][5];      /* admissible column patterns, max 31 */
static int  _vec_cnt = 0;

/* Pre‑compute transition table and vector list. */
static void _precompute(void) {
    if (_precomp_done) return;

    /* Decode each state (0..242) to its base‑3 digits. */
    for (int s = 0; s < STATE_CNT; ++s) {
        int r = s;
        for (int d = 0; d < 5; ++d) {
            _state_bits[s][d] = r % 3;
            r /= 3;
        }
    }

    /* Enumerate all 5‑bit patterns except 0, keep those whose value
       (with leftmost bit weight 2^4) is NOT divisible by 3. */
    for (unsigned int mask = 1; mask < (1U << 5); ++mask) {
        /* Compute value modulo 3.  Weights: 2^4,2^3,2^2,2^1,2^0
           => 1,2,1,2,1 mod 3.  Bits are indexed from left (0) to right (4). */
        int val_mod3 = 0;
        for (int d = 0; d < 5; ++d)
            val_mod3 += ((mask >> (4 - d)) & 1) * ((d % 2) ? 2 : 1);
        val_mod3 %= 3;
        if (val_mod3 != 0) {
            /* Store bit vector: bits in order left→right. */
            for (int d = 0; d < 5; ++d) {
                _vec[_vec_cnt][d] = (mask >> (4 - d)) & 1;
            }
            ++_vec_cnt;
        }
    }

    /* Build transitions for the two possible column weights
       (1 mod 3 and 2 mod 3). */
    for (int s = 0; s < STATE_CNT; ++s) {
        for (int v = 0; v < _vec_cnt; ++v) {
            /* weight 1 */
            int ns = 0, mul = 1;
            for (int d = 0; d < 5; ++d) {
                int nd = (_state_bits[s][d] + _vec[v][d]) % 3;
                ns += nd * mul;
                mul *= 3;
            }
            _trans[0][s][v] = ns;

            /* weight 2 (i.e. +2*vec) */
            ns = 0; mul = 1;
            for (int d = 0; d < 5; ++d) {
                int nd = (_state_bits[s][d] + 2 * _vec[v][d]) % 3;
                ns += nd * mul;
                mul *= 3;
            }
            _trans[1][s][v] = ns;
        }
    }

    _precomp_done = true;
}

/* Count the desired arrays. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset 1 */

    _precompute();

    idx_t m = n + 1;                 /* number of columns */

    static seq_t dp1[STATE_CNT];
    static seq_t dp2[STATE_CNT];

    /* Initialise DP arrays. */
    for (int i = 0; i < STATE_CNT; ++i) {
        dp1[i] = 0;
        dp2[i] = 0;
    }
    dp1[0] = 1;   /* start with all row sums 0 */

    seq_t *curr = dp1;
    seq_t *next = dp2;

    for (idx_t k = 0; k < m; ++k) {
        /* clear next state counts */
        for (int i = 0; i < STATE_CNT; ++i) next[i] = 0;

        /* column weight: 2^{m-1-k} mod 3 alternates 1,2,1,2,... */
        int weight = ((m - 1 - k) & 1) ? 2 : 1;
        int windex = (weight == 1) ? 0 : 1;

        for (int s = 0; s < STATE_CNT; ++s) {
            seq_t val = curr[s];
            if (val == 0) continue;
            for (int v = 0; v < _vec_cnt; ++v) {
                int t = _trans[windex][s][v];
                next[t] += val;
            }
        }

        /* swap current/next pointers */
        seq_t *tmp = curr;
        curr = next;
        next = tmp;
    }

    return curr[0];   /* only all‑zero residue state counts. */
}
