/*
 * A250009  Number of length n+5 0..3 arrays with no six consecutive terms
 *          having the maximum of any three terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A250009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+5 0..3 arrays with no six consecutive
 *            * terms having the maximum of any three terms equal to the
 *            minimum of the * remaining three terms. * * Computation is
 *            performed by dynamic programming over the last five * positions.
 *            Each state encodes a 5‑digit base‑4 number; at eac...
 * Range:     u8 none, u16 n <= 4, u32 n <= 16, u64 n <= 38, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define STATES 1024
#define MASK   1023  /* 0x3FF */

static bool table_built = false;
static bool validity[STATES][4];

/* Precompute popcount of numbers 0..63 */
static int popcount6[64] = {
    0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,
    1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
    1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
    2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6
};

static void build_validity_table(void)
{
    for (idx_t state = 0; state < STATES; ++state) {
        unsigned d[5];
        d[0] = (unsigned)((state >> 8) & 3); /* most significant digit (oldest) */
        d[1] = (unsigned)((state >> 6) & 3);
        d[2] = (unsigned)((state >> 4) & 3);
        d[3] = (unsigned)((state >> 2) & 3);
        d[4] = (unsigned)(state & 3);        /* least significant digit (newest) */

        for (unsigned t = 0; t < 4; ++t) {
            unsigned a[6] = { d[0], d[1], d[2], d[3], d[4], t };
            bool ok = true;
            for (int mask = 0; mask < 64; ++mask) {
                if (popcount6[mask] != 3) continue;
                unsigned maxS = 0;
                unsigned minC = 3; /* digits are 0..3 */
                for (int i = 0; i < 6; ++i) {
                    if (mask & (1 << i)) {
                        if (a[i] > maxS) maxS = a[i];
                    } else {
                        if (a[i] < minC) minC = a[i];
                    }
                }
                if (maxS == minC) {
                    ok = false;
                    break;
                }
            }
            validity[state][t] = ok;
        }
    }
    table_built = true;
}

seq_t a(idx_t n)
{
    if (!table_built) {
        build_validity_table();
    }

    if (n < 1) return 0;

    /* Length of the full array */
    idx_t L = n + 5;
    /* Number of extensions needed after the initial 5‑digit prefix */
    idx_t steps = L - 5;

    seq_t curr[STATES];
    seq_t next[STATES];

    /* Every 5‑digit prefix is initially possible */
    for (idx_t i = 0; i < STATES; ++i) {
        curr[i] = 1;
        next[i] = 0;
    }

    for (idx_t s = 0; s < steps; ++s) {
        for (idx_t m = 0; m < STATES; ++m) {
            seq_t val = curr[m];
            if (val == 0) continue;

            /* For each possible new digit */
            for (unsigned t = 0; t < 4; ++t) {
                if (!validity[m][t]) continue;
                idx_t newm = ((m << 2) | t) & MASK;
                next[newm] += val;
            }
        }
        /* Prepare for next step */
        for (idx_t i = 0; i < STATES; ++i) {
            curr[i] = next[i];
            next[i] = 0;
        }
    }

    /* Sum over all terminal states */
    seq_t result = 0;
    for (idx_t i = 0; i < STATES; ++i) {
        result += curr[i];
    }
    return result;
}
