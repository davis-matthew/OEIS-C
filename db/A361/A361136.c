/*
 * A361136  Numbers appearing on the upper face of a die as a result of its
 *          turning over the edge while it rolls along the square spiral of
 *          natural numbers.
 * https://oeis.org/A361136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simulation of die rolling on a clockwise square spiral (OEIS A361136) */
seq_t a(idx_t n)
{
    /* Initial top face before any moves */
    if (n <= 1)
        return 1;

    /* Die orientation (top, bottom, north, south, east, west) */
    seq_t U = 1, D = 6, N = 4, S = 3, E = 5, W = 2;

    idx_t steps  = 0;          /* number of moves performed */
    idx_t target = n - 1;      /* moves needed to reach a(n) */
    idx_t len    = 1;          /* current block length */

    while (steps < target)
    {
        /* East */
        for (idx_t i = 0; i < len && steps < target; ++i)
        {
            seq_t tmp = U;
            U = W;          /* top becomes west */
            W = D;          /* west becomes bottom */
            D = E;          /* bottom becomes east */
            E = tmp;        /* east becomes top */
            ++steps;
        }

        /* North */
        for (idx_t i = 0; i < len && steps < target; ++i)
        {
            seq_t tmp = U;
            U = S;          /* top becomes south */
            S = D;          /* south becomes bottom */
            D = N;          /* bottom becomes north */
            N = tmp;        /* north becomes top */
            ++steps;
        }

        ++len;               /* next two directions use a longer block */

        /* West */
        for (idx_t i = 0; i < len && steps < target; ++i)
        {
            seq_t tmp = U;
            U = E;          /* top becomes east */
            E = D;          /* east becomes bottom */
            D = W;          /* bottom becomes west */
            W = tmp;        /* west becomes top */
            ++steps;
        }

        /* South */
        for (idx_t i = 0; i < len && steps < target; ++i)
        {
            seq_t tmp = U;
            U = N;          /* top becomes north */
            N = D;          /* north becomes bottom */
            D = S;          /* bottom becomes south */
            S = tmp;        /* south becomes top */
            ++steps;
        }

        ++len;               /* prepare for the next pair of directions */
    }

    return U;   /* top face after n-1 rolls */
}
