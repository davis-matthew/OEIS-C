/*
 * A263056  Number of (n+1)X(4+1) 0..1 arrays with each row and column not
 *          divisible by 3, read as a binary number with top and left being
 *          the most significant bits.
 * https://oeis.org/A263056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts (n+1)×5 binary arrays in which every row and every column,
   interpreted as a binary number with most significant bit first,
   is not divisible by 3.  Computed by dynamic programming over the
   column residues modulo 3.  Initial values: a(1)=10, a(2)=895, a(3)=18390, ... */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Number of states: each column has residue 0,1,2 → 3^5 = 243 */
    enum { STATES = 243, ROWS   = 21 };
    enum { MAX_ROWS = 32 };

    /* static data, initialized once */
    static bool initialized = false;
    static int  next_even[STATES][ROWS];
    static int  next_odd [STATES][ROWS];
    static bool accept   [STATES];
    static int  row_bits [ROWS][5];
    static seq_t dp[2][STATES];

    if (!initialized)
    {
        /* ---------- admissible rows (21 masks) ---------- */
        const int weight[5] = {1, 2, 1, 2, 1};     /* binary weights mod 3 */
        int count = 0;
        for (int mask = 0; mask < MAX_ROWS; ++mask)
        {
            int ssum = 0;
            for (int j = 0; j < 5; ++j)
            {
                int bit = (mask >> (4 - j)) & 1;    /* leftmost is MSB */
                row_bits[count][j] = bit;
                ssum += bit * weight[j];
            }
            if ((ssum % 3) != 0)   /* row integer not divisible by 3 */
                ++count;
        }

        /* ---------- pre‑compute transitions and accept states ---------- */
        for (int s = 0; s < STATES; ++s)
        {
            /* decode state s into 5 base‑3 digits (column residues) */
            int digits[5];
            int tmp = s;
            for (int j = 4; j >= 0; --j)
            {
                digits[j] = tmp % 3;
                tmp /= 3;
            }

            /* state is accepting if all residues are non‑zero */
            accept[s] = (digits[0] && digits[1] && digits[2] && digits[3] && digits[4]);

            for (int p = 0; p < ROWS; ++p)
            {
                /* even row (from bottom): coefficient 1 → add bit */
                int new_d[5];
                for (int j = 0; j < 5; ++j)
                    new_d[j] = (digits[j] + row_bits[p][j]) % 3;
                int idx = 0;
                for (int j = 0; j < 5; ++j) idx = idx * 3 + new_d[j];
                next_even[s][p] = idx;

                /* odd row: coefficient 2 → add 2·bit */
                for (int j = 0; j < 5; ++j)
                    new_d[j] = (digits[j] + (row_bits[p][j] * 2)) % 3;
                idx = 0;
                for (int j = 0; j < 5; ++j) idx = idx * 3 + new_d[j];
                next_odd[s][p] = idx;
            }
        }
        initialized = true;
    }

    /* ---------- dynamic program over rows ---------- */
    int L = (int)(n + 1);               /* number of rows */
    int cur = 0, nxt = 1;

    for (int i = 0; i < STATES; ++i)
        dp[cur][i] = dp[nxt][i] = 0;
    dp[cur][0] = 1;                     /* all column residues start at 0 */

    for (int row = 0; row < L; ++row)
    {
        bool even = (row % 2 == 0);      /* even row counted from bottom */

        for (int i = 0; i < STATES; ++i)
            dp[nxt][i] = 0;

        for (int s = 0; s < STATES; ++s)
        {
            seq_t val = dp[cur][s];
            if (val == 0) continue;
            int *trans = even ? next_even[s] : next_odd[s];
            for (int p = 0; p < ROWS; ++p)
                dp[nxt][trans[p]] += val;
        }
        int tmp = cur; cur = nxt; nxt = tmp;
    }

    /* sum over accepting final states */
    seq_t ans = 0;
    for (int s = 0; s < STATES; ++s)
        if (accept[s]) ans += dp[cur][s];

    return ans;
}
