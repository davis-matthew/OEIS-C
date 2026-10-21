/*
 * A279745  Number of 5Xn 0..1 arrays with no element equal to a strict
 *          majority of its horizontal and antidiagonal neighbors, with the
 *          exception of exactly one element, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A279745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of 5×n 0/1 arrays with exactly one element
 * equal to a strict majority of its horizontal and antidiagonal
 * neighbors.
 * The algorithm uses a transfer‑matrix dynamic programming that
 * keeps track of the last two columns and whether a violation has
 * already been encountered.  It counts each valid array exactly twice,
 * hence the result is divided by two at the end.
 */

#include <stddef.h>
#include <stdbool.h>

/* index representing a non‑existent neighbour */
#define DUMMY 32          /* 0..31 are column bitmasks */

/* ------------------------------------------------------------------ */
/* compute the number of violating cells in column `curr` given          */
/* neighbours `prev` (left) and `next` (right).                         */
/* ------------------------------------------------------------------ */
static unsigned char
compute_violation(uint16_t prev, uint16_t curr, uint16_t next)
{
    unsigned char vcnt = 0;
    bool leftPresent  = (prev  != DUMMY);
    bool rightPresent = (next  != DUMMY);

    for (int r = 0; r < 5; ++r)
    {
        int N    = 0;       /* number of existing neighbours              */
        int same = 0;       /* neighbours equal to the cell's bit         */
        int bit  = (curr >> r) & 1;

        /* left neighbour                                 */
        if (leftPresent)
        {
            ++N;
            if (((prev >> r) & 1) == bit) ++same;
        }

        /* right neighbour                                */
        if (rightPresent)
        {
            ++N;
            if (((next >> r) & 1) == bit) ++same;
        }

        /* down‑left neighbour (prev, r+1)                */
        if (leftPresent && r < 4)
        {
            ++N;
            if (((prev >> (r + 1)) & 1) == bit) ++same;
        }

        /* up‑right neighbour (next, r‑1)                 */
        if (rightPresent && r > 0)
        {
            ++N;
            if (((next >> (r - 1)) & 1) == bit) ++same;
        }

        if (N > 0 && 2 * same > N)          /* strict majority?   */
            ++vcnt;
    }
    return vcnt;
}

/* ------------------------------------------------------------------ */
/* main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* a(1) = 0 by definition */
    if (n < 2)
        return 0;

    /* --------------------------------------------------------------- */
    /* pre‑compute violation counts for all column triples             */
    /* --------------------------------------------------------------- */
    static unsigned char V[33][32][33];
    static bool V_inited = false;
    if (!V_inited)
    {
        for (int prev = 0; prev <= 32; ++prev)
            for (int curr = 0; curr < 32; ++curr)
                for (int next = 0; next <= 32; ++next)
                    V[prev][curr][next] = compute_violation(prev, curr, next);
        V_inited = true;
    }

    /* --------------------------------------------------------------- */
    /* DP tables: dp0[prev][curr] – no violation yet                  */
    /*           dp1[prev][curr] – one violation already used        */
    /* --------------------------------------------------------------- */
    static seq_t dp0[32][32];
    static seq_t dp1[32][32];
    static seq_t ndp0[32][32];
    static seq_t ndp1[32][32];
    seq_t one = 1;

    /* initialise with all entries zero */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
        {
            dp0[i][j] = 0;
            dp1[i][j] = 0;
        }

    /* initial transition: process column 1 (prev = dummy) */
    for (int c1 = 0; c1 < 32; ++c1)
    {
        for (int c2 = 0; c2 < 32; ++c2)
        {
            unsigned char v1 = V[DUMMY][c1][c2];
            if (v1 > 1)          /* more than one violating cell in column 1 */
                continue;
            if (v1 == 0)
                dp0[c1][c2] += one;
            else                  /* v1 == 1 */
                dp1[c1][c2] += one;
        }
    }

    /* ---------------------------------------------------------------- */
    /* process columns 3 … n (n >= 3)                                   */
    /* ---------------------------------------------------------------- */
    for (int step = 3; step <= (int)n; ++step)
    {
        /* zero next DP arrays */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
            {
                ndp0[i][j] = 0;
                ndp1[i][j] = 0;
            }

        for (int prev = 0; prev < 32; ++prev)
        {
            for (int curr = 0; curr < 32; ++curr)
            {
                seq_t val0 = dp0[prev][curr];
                seq_t val1 = dp1[prev][curr];
                if (!val0 && !val1)
                    continue;

                for (int next = 0; next < 32; ++next)
                {
                    unsigned char v = V[prev][curr][next];
                    if (v > 1)               /* two or more violations in curr */
                        continue;

                    if (val0)
                    {
                        if (v == 0)
                            ndp0[curr][next] += val0;
                        else                /* v == 1 */
                            ndp1[curr][next] += val0;
                    }

                    if (val1 && v == 0)      /* already used violation      */
                        ndp1[curr][next] += val1;
                }
            }
        }

        /* swap dp and ndp */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
            {
                dp0[i][j] = ndp0[i][j];
                dp1[i][j] = ndp1[i][j];
            }
    }

    /* ---------------------------------------------------------------- */
    /* final column (n) handling                                       */
    /* ---------------------------------------------------------------- */
    seq_t ans = 0;
    for (int prev = 0; prev < 32; ++prev)
    {
        for (int curr = 0; curr < 32; ++curr)
        {
            seq_t val0 = dp0[prev][curr];
            seq_t val1 = dp1[prev][curr];
            if (!val0 && !val1)
                continue;

            unsigned char v_last = V[prev][curr][DUMMY];
            if (val0 && v_last == 1)   /* last column gives the single violation */
                ans += val0;
            if (val1 && v_last == 0)   /* violation already used earlier        */
                ans += val1;
        }
    }

    /* each valid array has been counted twice – divide by two          */
    return ans / 2;
}
