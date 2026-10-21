/*
 * A252059  Number of (n+2)X(2+2) 0..2 arrays with every 3X3 subblock row,
 *          column, diagonal and antidiagonal sum not equal to 1 2 or 4.
 * https://oeis.org/A252059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 18, u64 n <= 39, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A252059
 *  a(n) = number of (n+2)×4 arrays with entries in {0,1,2}
 *  such that every 3×3 subblock has no row, column, diagonal
 *  or antidiagonal sum equal to 1, 2 or 4.
 *
 *  The implementation uses a linear‑time DP with a state consisting
 *  of the last two rows (each encoded as a number 0…80).  For each
 *  pair (a,b) the possible next rows c are pre‑computed; the DP
 *  then iterates from row 3 up to n+2, accumulating the number of
 *  valid arrays.
 *
 *  The function works for any seq_t / idx_t supplied by the build.
 *  It avoids dynamic allocation (which would break for arbitrary‑precision
 *  seq_t types) by using static arrays.  All operations respect the
 *  restrictions of the problem statement.
 */

#include <stdbool.h>
#include <string.h>

static unsigned char col0[81], col1[81], col2[81], col3[81];
static unsigned char row0sum[81], row1sum[81];
static unsigned char deg[81 * 81];
static unsigned char next_row[81 * 81][81];
static bool tables_built = false;

/* Build transition table once.  No dynamic memory is used. */
static void build_tables(void)
{
    if (tables_built)
        return;

    /* pre‑compute digits and partial sums for each 4‑digit row */
    for (int p = 0; p < 81; ++p) {
        int tmp = p;
        unsigned char d0 = tmp % 3; tmp /= 3;
        unsigned char d1 = tmp % 3; tmp /= 3;
        unsigned char d2 = tmp % 3; tmp /= 3;
        unsigned char d3 = tmp;              /* 0..2 */
        col0[p] = d0; col1[p] = d1;
        col2[p] = d2; col3[p] = d3;
        row0sum[p] = d0 + d1 + d2;           /* columns 0–2 */
        row1sum[p] = d1 + d2 + d3;           /* columns 1–3 */
    }

    /* For every pair (a,b) compute admissible next rows c */
    for (int a = 0; a < 81; ++a) {
        for (int b = 0; b < 81; ++b) {
            int base = a * 81 + b;
            unsigned char d = 0;

            for (int c = 0; c < 81; ++c) {
                /* 3×3 block columns 0–2 */
                int rs0 = row0sum[a];
                int rs1 = row0sum[b];
                int rs2 = row0sum[c];
                if (rs0 == 1 || rs0 == 2 || rs0 == 4) continue;
                if (rs1 == 1 || rs1 == 2 || rs1 == 4) continue;
                if (rs2 == 1 || rs2 == 2 || rs2 == 4) continue;

                int cs0 = col0[a] + col0[b] + col0[c];
                int cs1 = col1[a] + col1[b] + col1[c];
                int cs2 = col2[a] + col2[b] + col2[c];
                if (cs0 == 1 || cs0 == 2 || cs0 == 4) continue;
                if (cs1 == 1 || cs1 == 2 || cs1 == 4) continue;
                if (cs2 == 1 || cs2 == 2 || cs2 == 4) continue;

                int diag0 = col0[a] + col1[b] + col2[c];
                int anti0 = col2[a] + col1[b] + col0[c];
                if (diag0 == 1 || diag0 == 2 || diag0 == 4) continue;
                if (anti0 == 1 || anti0 == 2 || anti0 == 4) continue;

                /* 3×3 block columns 1–3 */
                int rs3 = row1sum[a];
                int rs4 = row1sum[b];
                int rs5 = row1sum[c];
                if (rs3 == 1 || rs3 == 2 || rs3 == 4) continue;
                if (rs4 == 1 || rs4 == 2 || rs4 == 4) continue;
                if (rs5 == 1 || rs5 == 2 || rs5 == 4) continue;

                int cs3 = col1[a] + col1[b] + col1[c];
                int cs4 = col2[a] + col2[b] + col2[c];
                int cs5 = col3[a] + col3[b] + col3[c];
                if (cs3 == 1 || cs3 == 2 || cs3 == 4) continue;
                if (cs4 == 1 || cs4 == 2 || cs4 == 4) continue;
                if (cs5 == 1 || cs5 == 2 || cs5 == 4) continue;

                int diag1 = col1[a] + col2[b] + col3[c];
                int anti1 = col3[a] + col2[b] + col1[c];
                if (diag1 == 1 || diag1 == 2 || diag1 == 4) continue;
                if (anti1 == 1 || anti1 == 2 || anti1 == 4) continue;

                /* admissible next row */
                next_row[base][d++] = (unsigned char)c;
            }
            deg[base] = d;
        }
    }
    tables_built = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* sequence starts at n = 1 */

    build_tables();

    idx_t h = n + 2;            /* total number of rows */

    /* static arrays to hold DP counts (size 81×81 = 6561) */
    static seq_t curr[81 * 81];
    static seq_t nxt[81 * 81];
    seq_t *currp = curr;
    seq_t *nextp = nxt;

    /* initialize all state counts to zero */
    memset(currp, 0, sizeof(curr));
    /* all pairs of first two rows are initially valid */
    for (int a = 0; a < 81; ++a)
        for (int b = 0; b < 81; ++b)
            currp[a * 81 + b] = 1;

    /* DP: add rows 3 … h */
    for (idx_t row = 3; row <= h; ++row) {
        /* clear next array */
        memset(nextp, 0, sizeof(nxt));

        for (int a = 0; a < 81; ++a) {
            for (int b = 0; b < 81; ++b) {
                int base = a * 81 + b;
                seq_t cnt = currp[base];
                if (!cnt)
                    continue;
                unsigned char d = deg[base];
                unsigned char *nbr = next_row[base];
                for (int k = 0; k < d; ++k) {
                    int c = nbr[k];
                    int next_idx = b * 81 + c;
                    nextp[next_idx] += cnt;
                }
            }
        }
        /* swap arrays for next iteration */
        seq_t *tmp = currp;
        currp = nextp;
        nextp = tmp;
    }

    /* sum all valid end states */
    seq_t total = 0;
    for (int i = 0; i < 81 * 81; ++i)
        total += currp[i];

    return total;
}
