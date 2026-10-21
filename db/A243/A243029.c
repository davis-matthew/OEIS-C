/*
 * A243029  Number of length n+3 0..4 arrays with no four elements in a row
 *          with pattern abba (possibly a=b) and new values 0..4 introduced in
 *          0..4 order.
 * https://oeis.org/A243029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A243029: number of length n+3 arrays over {0..4} with the RGF rule
 * (new values introduced in 0..4 order) and no four consecutive elements
 * of the pattern a,b,b,a (a may equal b).  The function computes the
 * sequence by dynamic programming over the last three values and the
 * current maximum value.  The recurrence is implicit in the DP; no
 * explicit recurrence is given in the OEIS entry.  All intermediate
 * values are held in type seq_t, which may be an arbitrary‑precision
 * integer in the test harness.  The algorithm runs in O(n) time and
 * uses O(1080) memory for the DP state. */

/* ---------- constants and helper ------------------------------------ */

#define LAST_VAL 6          /* values 0..4 and special marker 5        */
#define MAX_VAL  5          /* maximum possible value (0..4)            */
#define STATE_SLOTS (LAST_VAL * LAST_VAL * LAST_VAL * MAX_VAL)

/* compute an index into the DP arrays */
static inline int
state_index (int l1, int l2, int l3, int mx)
{
    return (((l1 * LAST_VAL + l2) * LAST_VAL + l3) * MAX_VAL + mx);
}

/* ---------- sequence function -------------------------------------- */

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/* external linkage as required */
seq_t a ( idx_t n )
{
    /* length of arrays to count */
    idx_t L = n + 3;

    /* static state for the DP */
    static seq_t dp_cur[STATE_SLOTS];
    static seq_t dp_next[STATE_SLOTS];
    static idx_t current_len = 0;          /* maximum length already processed */

    /* table of totals[ℓ] = number of arrays of length ℓ */
    static seq_t *totals = NULL;
    static size_t totals_cap = 0;          /* allocated capacity               */
    static size_t totals_len = 0;          /* number of stored entries         */

    /* initialise everything on first call */
    if ( totals == NULL )
    {
        totals_cap = 64;                  /* initial capacity                 */
        totals = (seq_t *)calloc ( totals_cap, sizeof ( seq_t ) );
        if ( totals == NULL ) return 0;   /* out of memory – unlikely */
        memset ( dp_cur, 0, sizeof ( dp_cur ) );
        dp_cur[state_index (5,5,5,0)] = 1; /* empty array                    */
        totals[0] = 1;                    /* length 0: 1 empty array          */
        totals_len = 1;
        current_len = 0;
    }

    /* grow the totals array if needed */
    if ( totals_len <= (size_t)L )
    {
        size_t new_cap = totals_cap;
        while ( new_cap <= (size_t)L ) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc ( totals, new_cap * sizeof ( seq_t ) );
        if ( tmp == NULL ) return 0;       /* out of memory – unlikely */
        totals = tmp;
        totals_cap = new_cap;
        /* initialise new entries to zero */
        memset ( &totals[totals_len], 0, (new_cap - totals_len) * sizeof ( seq_t ) );
    }

    /* extend the DP until we reach length L */
    for ( idx_t pos = current_len + 1; pos <= L; ++pos )
    {
        memset ( dp_next, 0, sizeof ( dp_next ) );

        for ( int l1 = 0; l1 < LAST_VAL; ++l1 )
        {
            for ( int l2 = 0; l2 < LAST_VAL; ++l2 )
            {
                for ( int l3 = 0; l3 < LAST_VAL; ++l3 )
                {
                    for ( int mx = 0; mx < MAX_VAL; ++mx )
                    {
                        seq_t cur_cnt = dp_cur[ state_index ( l1, l2, l3, mx ) ];
                        if ( cur_cnt == 0 ) continue;

                        int max_allowed = 0;
                        if ( pos == 1 )
                        {
                            max_allowed = 0;              /* first element must be 0 */
                        }
                        else
                        {
                            int lim = mx + 1;              /* allow new value */
                            if ( lim > 4 ) lim = 4;
                            max_allowed = lim;
                        }

                        for ( int val = 0; val <= max_allowed; ++val )
                        {
                            /* forbid pattern a,b,b,a with a=val, b=l2=l3 */
                            if ( l1 != 5 && l1 == val && l2 == l3 ) continue;

                            int nl1 = l2;
                            int nl2 = l3;
                            int nl3 = val;
                            int nmx = mx;
                            if ( val > nmx ) nmx = val;

                            dp_next[ state_index ( nl1, nl2, nl3, nmx ) ] += cur_cnt;
                        }
                    }
                }
            }
        }

        /* copy dp_next to dp_cur for next iteration */
        memcpy ( dp_cur, dp_next, sizeof ( dp_cur ) );

        /* compute total for this length */
        seq_t total = 0;
        for ( size_t s = 0; s < STATE_SLOTS; ++s )
            total += dp_cur[s];

        totals[pos] = total;
        if ( pos >= totals_len ) totals_len = pos + 1;
    }

    current_len = L;                       /* remember the longest processed length */
    return totals[L];
}
