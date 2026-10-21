/*
 * A285398  Start with a single cell at coordinates (0, 0, 0), then
 *          iteratively subdivide the grid into 3 X 3 X 3 cells and remove the
 *          cells whose sum of modulo 2 coordinates is 0; a(n) is the number
 *          of cells after n iterations.
 * https://oeis.org/A285398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A285398: a(n) = number of cells after n successive
   3×3×3 subdivisions with removal of cells whose\n
   all coordinates are even.
   Recurrence: a(0)=1, a(1)=19, a(2)=452.
   Computing method: 4‑state dynamic programming that tracks
   the counts of cells with 0,1,2 or 3 odd coordinates at each
   level.  Transition table CT[r][s] (for r,s∈{0,1,2,3}) gives the
   number of children with s odd coordinates produced from a parent
   that had r odd coordinates.  The children with s=0 are removed.
   The total a(n) is the sum of the 4 state counts at level n.  The
   function memoises all intermediate states so that repeated calls
   are O(1) after the first evaluation.
*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static const unsigned char CT[4][4] = {
    /*   0   1   2   3 */
    {   0, 12,  6,  1 },   /* r = 0 */
    {   0, 12,  9,  2 },   /* r = 1 */
    {   0,  9, 12,  4 },   /* r = 2 */
    {   0,  6, 12,  8 }    /* r = 3 */
};

struct State {
    seq_t r[4];
};

static struct State *states      = NULL;   /* memoised DP states   */
static idx_t        states_len   = 0;      /* number of states stored */
static idx_t        states_cap   = 0;      /* capacity of array     */

/* Ensure that states[0..n] have been computed.  If the current
   capacity is insufficient, a new larger array is allocated,
   existing data are copied and the new states up to n are
   computed.  The function returns 0 on allocation failure,
   otherwise returns 1. */
static int ensure_states(idx_t n)
{
    if (n + 1 > states_cap) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < 4) new_cap = 4;               /* minimal capacity */
        struct State *tmp = (struct State *)calloc(new_cap, sizeof(struct State));
        if (!tmp)
            return 0;                              /* allocation failure */
        if (states) {
            memcpy(tmp, states, sizeof(struct State) * states_len);
            free(states);
        }
        states = tmp;
        states_cap = new_cap;
    }

    if (states_len == 0) {                        /* first state */
        for (int r = 0; r < 4; ++r)
            states[0].r[r] = (seq_t)(r == 0);
        states_len = 1;
    }

    while (states_len <= n) {
        seq_t next[4] = { 0, 0, 0, 0 };
        const seq_t *prev = states[states_len - 1].r;
        for (int r = 0; r < 4; ++r) {
            seq_t pr = prev[r];
            if (!pr) continue;
            for (int s = 1; s < 4; ++s) {
                next[s] += pr * (seq_t)CT[r][s];
            }
        }
        for (int s = 0; s < 4; ++s)
            states[states_len].r[s] = next[s];
        ++states_len;
    }
    return 1;
}

seq_t a(idx_t n)
{
    if (!ensure_states(n))
        return (seq_t)0;                           /* fallback on failure */

    seq_t total = 0;
    for (int r = 0; r < 4; ++r)
        total += states[n].r[r];
    return total;
}
