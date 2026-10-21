/*
 * A262328  Number of (n+1) X (4+1) 0..1 arrays with each row and column
 *          divisible by 3, read as a binary number with top and left being
 *          the most significant bits.
 * https://oeis.org/A262328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of (n+1)×5 binary arrays whose each row and
 *            each column, read as a binary number with the left/top being the
 *            most significant bits, is divisible by 3. It satisfies the
 *            recurrence a(n) = 11*a(n‑1) + 48*a(n‑2) – 528*a(n‑3) –
 *            579*a(n‑4) + 6369*a(n‑5) + 1612*a(n‑6) – 17732*a(n‑7)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* minimal includes for C11 / C++17 */
#include <stdint.h>
#include <stdbool.h>

/* 5‑bit binary patterns whose value is divisible by 3 (11 patterns) */
static const unsigned char patterns[11][5] = {
    {0,0,0,0,0},
    {1,0,1,0,1},
    {1,1,0,0,0},
    {1,0,0,1,0},
    {0,1,1,0,0},
    {0,0,1,1,0},
    {0,1,0,0,1},
    {0,0,0,1,1},
    {1,1,1,1,0},
    {1,1,0,1,1},
    {0,1,1,1,1}
};

/* transition tables: trans_even[s][p] and trans_odd[s][p] */
static uint16_t trans_even[243][11];
static uint16_t trans_odd[243][11];
static bool trans_initialized = false;

/* encode 5 base‑3 digits (0..2) into a number 0..242 */
static inline uint16_t encode(const unsigned char d[5])
{
    return (uint16_t)(d[0] + 3*d[1] + 9*d[2] + 27*d[3] + 81*d[4]);
}

/* initialise transition tables once */
static void init_trans(void)
{
    if (trans_initialized) return;
    for (uint16_t s = 0; s < 243; ++s) {
        unsigned char d[5];
        unsigned int tmp = s;
        for (int k = 0; k < 5; ++k) {
            d[k] = tmp % 3;
            tmp /= 3;
        }
        for (int p = 0; p < 11; ++p) {
            unsigned char nd[5];
            /* even row: weight 1 */
            for (int j = 0; j < 5; ++j)
                nd[j] = (d[j] + patterns[p][j]) % 3;
            trans_even[s][p] = encode(nd);
            /* odd row: weight 2 */
            for (int j = 0; j < 5; ++j)
                nd[j] = (d[j] + (2 * patterns[p][j]) % 3) % 3;
            trans_odd[s][p] = encode(nd);
        }
    }
    trans_initialized = true;
}

/* external linkage required by the problem */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset starts at 1 */
    init_trans();

    const idx_t rows = n + 1;      /* (n+1) rows */

    seq_t dp[243];
    seq_t next[243];

    /* initialise dp */
    for (int i = 0; i < 243; ++i) dp[i] = 0;
    dp[0] = (seq_t)1;             /* all column residues zero */

    for (idx_t r = 0; r < rows; ++r) {
        /* zero next */
        for (int i = 0; i < 243; ++i) next[i] = 0;

        /* select transition table depending on row parity */
        uint16_t (*trans)[11] = (r & 1) ? trans_odd : trans_even;

        for (unsigned int s = 0; s < 243; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;
            for (int p = 0; p < 11; ++p) {
                unsigned int ns = trans[s][p];
                next[ns] = next[ns] + val;
            }
        }

        /* copy next to dp for next iteration */
        for (int i = 0; i < 243; ++i) dp[i] = next[i];
    }

    return dp[0];                  /* final column residues all zero */
}
