/*
 * A242470  Number of length n+2 0..7 arrays with no three equal elements in a
 *          row and new values 0..7 introduced in 0..7 order.
 * https://oeis.org/A242470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of length n+2 arrays over {0,…,7} with no three equal
 *            consecutive elements and with new symbols introduced in the
 *            natural order. Computed via dynamic programming over the state
 *            (max_used, last1, last2) with transitions that respect the
 *            rules. The state space has 8*8*8 = 512 states. Complex...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* fixed number of DP states: max_used (0..7), last1 (0..7), last2 (0..7) */
#define STATE_COUNT 512

static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;

/* ensure that the DP arrays are allocated once */
static void init_dp_arrays(void)
{
    if (dp_cur == NULL)
    {
        dp_cur  = (seq_t *)malloc(STATE_COUNT * sizeof(seq_t));
        dp_next = (seq_t *)malloc(STATE_COUNT * sizeof(seq_t));
        /* zero them */
        for (int i = 0; i < STATE_COUNT; ++i)
        {
            dp_cur[i]  = 0;
            dp_next[i] = 0;
        }
    }
}

seq_t a(idx_t n)
{
    /* n >= 1 by specification */
    if (n == 0) return 0;          /* undefined for offset < 1, safety */

    init_dp_arrays();

    /* reset current DP layer */
    for (int i = 0; i < STATE_COUNT; ++i)
        dp_cur[i] = 0;

    /* initial states after first two positions:
       state 0: [0,0]   -> max_used=0, last1=0, last2=0
       state 72 (1<<6 + 1<<3): [0,1] -> max_used=1, last1=1, last2=0
    */
    dp_cur[0]  = 1;          /* [0,0] */
    dp_cur[72] = 1;          /* [0,1] */

    /* total length L = n + 2 */
    idx_t L = n + 2;
    /* we already have length 2, so add symbols until position L */
    for (idx_t step = 3; step <= L; ++step)
    {
        /* zero next layer */
        for (int i = 0; i < STATE_COUNT; ++i)
            dp_next[i] = 0;

        for (int idx = 0; idx < STATE_COUNT; ++idx)
        {
            seq_t cnt = dp_cur[idx];
            if (cnt == 0) continue;   /* skip empty states */

            int max_used = idx >> 6;          /* upper 3 bits */
            int last1    = (idx >> 3) & 7;    /* next 3 bits */
            int last2    = idx & 7;           /* lower 3 bits */

            /* Try existing symbols 0..max_used */
            for (int sym = 0; sym <= max_used; ++sym)
            {
                if (last1 == last2 && last1 == sym) continue;   /* would form triple */

                int new_idx = ((max_used << 6) | (sym << 3) | last1);
                dp_next[new_idx] += cnt;
            }

            /* Possibly introduce new symbol max_used+1 */
            if (max_used < 7)
            {
                int sym = max_used + 1;
                if (!(last1 == last2 && last1 == sym))   /* triple check not needed actually */
                {
                    int new_idx = (((max_used + 1) << 6) | (sym << 3) | last1);
                    dp_next[new_idx] += cnt;
                }
            }
        }

        /* swap layers */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* sum all finishing states */
    seq_t total = 0;
    for (int i = 0; i < STATE_COUNT; ++i)
        total += dp_cur[i];

    return total;
}
