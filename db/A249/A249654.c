/*
 * A249654  Number of length n+4 0..6 arrays with every five consecutive terms
 *          having four times some element equal to the sum of the remaining
 *          four.
 * https://oeis.org/A249654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 15, u32 n <= 55, u64 n <= 134, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A249654
    a(n) = number of length n+4 0..6 arrays such that every set of five consecutive
    terms contains an element that is the average of the other four
    (i.e. 4×x = sum of the other four, equivalently 5×x = total).
    The function computes the terms by dynamic programming on 4‑digit
    states.  The recurrence is implicit: for each 4‑digit state s the
    possible next states are determined by all 5‑tuples (s, x) satisfying
    the condition.  This gives a linear‑time algorithm O(n·|S|), where
    |S| is the number of valid 5‑tuples (2077). */

#include <stddef.h>

/*  The returned type is supplied by the build.  We do not declare it here.  */
seq_t a(idx_t n)
{
    /*  A 5‑tuple is valid iff its sum is a multiple of 5 and one entry
        equals the common value sum/5.  We pre‑compute all transitions
        between 4‑digit states.  The maximum number of states is 7^4 = 2401
        and each state has at most 7 outgoing transitions.  */
    static unsigned char  next_cnt[2401] = {0};          /* number of outgoing
                                                          transitions per state      */
    static int            next_state[2401][7] = {{0}};    /* target states           */
    static int            initialized          = 0;       /* one‑time flag          */

    if ( !initialized )
    {
        /*  Enumerate all 5‑tuples (a,b,c,d,e) in [0..6] and add a
            transition from 4–tuple (a,b,c,d) to (b,c,d,e) when the
            5‑tuple is valid.  */
        for (int a = 0; a < 7; ++a)
        for (int b = 0; b < 7; ++b)
        for (int c = 0; c < 7; ++c)
        for (int d = 0; d < 7; ++d)
        for (int e = 0; e < 7; ++e)
        {
            int sum = a + b + c + d + e;
            if ( sum % 5 != 0 )               continue;
            int q = sum / 5;
            if ( q != a && q != b && q != c &&
                 q != d && q != e )          continue;

            int prev = ( ( (a*7 + b)*7 + c )*7 + d );   /* state of a,b,c,d        */
            int nxt  = ( ( (b*7 + c)*7 + d )*7 + e );   /* state of b,c,d,e        */

            unsigned char k = next_cnt[prev];
            next_state[prev][k] = nxt;
            next_cnt[prev]   = k + 1;
        }
        initialized = 1;
    }

    /*  DP arrays over the 4‑digit states.  We keep two vectors
        cur and nxt; each step adds one more digit.  */
    static seq_t cur[2401];
    static seq_t nxt[2401];

    /*  Start from any 4‑digit prefix: one way for every state.  */
    for (int i = 0; i < 2401; ++i) cur[i] = 1;

    /*  We must add n digits after the initial 4, i.e. perform n steps.  */
    for (idx_t step = 1; step <= n; ++step)
    {
        /*  Initialise the next vector with zeros.  */
        for (int i = 0; i < 2401; ++i) nxt[i] = 0;

        /*  Propagate all transitions.  */
        for (int s = 0; s < 2401; ++s)
        {
            seq_t val = cur[s];
            if ( !val ) continue;                 /* state unreachable so far */

            unsigned char k = next_cnt[s];
            for (unsigned char j = 0; j < k; ++j)
            {
                int t = next_state[s][j];
                nxt[t] += val;
            }
        }

        /*  Prepare for the next iteration.  */
        for (int i = 0; i < 2401; ++i) cur[i] = nxt[i];
    }

    /*  Sum the counts of all possible last states.  */
    seq_t total = 0;
    for (int i = 0; i < 2401; ++i) total += cur[i];

    return total;
}
