/*
 * A250568  Number of (n+2)X(2+2) 0..1 arrays with nondecreasing maximum of
 *          every three consecutive values in every row and column.
 * https://oeis.org/A250568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A250568: Number of (n+2)×4 0..1 arrays with nondecreasing maximum of
   every three consecutive values in each row and each column. */
#include <stdbool.h>

/* Transition table for a single column automaton.
   States: 0 – no 1 seen yet,
           1 – seen 1, trailing zeros count 0,
           2 – seen 1, trailing zeros count 1,
           3 – seen 1, trailing zeros count 2.
   Out of state 3, input 0 is forbidden. */
static const signed char COL_TRANS[4][2] = {
    { 0, 1 },    /* state 0  */
    { 2, 1 },    /* state 1  */
    { 3, 1 },    /* state 2  */
    { -1, 1 }    /* state 3  */
};

/* Precomputed 256×15 transition table for the whole matrix.
   next_state[s][k] gives the destination state after adding an allowed
   row pattern (except 1000).  next_len[s] = number of valid transitions. */
static uint8_t next_state[256][15];
static uint8_t next_len[256];
static bool init_done = false;

/* Build the transition tables once. */
static void init_transitions(void)
{
    if (init_done) return;
    init_done = true;
    for (int s = 0; s < 256; ++s) {
        int s0 = s & 3;
        int s1 = (s >> 2) & 3;
        int s2 = (s >> 4) & 3;
        int s3 = (s >> 6) & 3;
        uint8_t len = 0;
        for (int p = 0; p < 16; ++p) {
            if (p == 8) continue;          /* row pattern 1000 is forbidden */
            int b0 =  (p >> 0) & 1;
            int b1 =  (p >> 1) & 1;
            int b2 =  (p >> 2) & 1;
            int b3 =  (p >> 3) & 1;
            int nb0 = COL_TRANS[s0][b0];
            int nb1 = COL_TRANS[s1][b1];
            int nb2 = COL_TRANS[s2][b2];
            int nb3 = COL_TRANS[s3][b3];
            if (nb0 < 0 || nb1 < 0 || nb2 < 0 || nb3 < 0) continue; /* invalid */
            uint8_t ns = (uint8_t)(nb0 | (nb1 << 2) | (nb2 << 4) | (nb3 << 6));
            next_state[s][len++] = ns;
        }
        next_len[s] = len;
    }
}

/* Main function computing a(n). */
seq_t a(idx_t n)
{
    init_transitions();                /* ensure tables built */

    idx_t N = n + 2;                   /* number of rows in the matrix */

    /* dp[256] holds counts of ways to obtain each state after current number
       of rows.  Use two arrays and alternate pointers for memory efficiency. */
    seq_t dp[256] = {0};
    seq_t tmp[256] = {0};
    seq_t *cur = dp, *nxt = tmp;

    /* --- initial processing: first row --- */
    for (uint8_t k = 0; k < next_len[0]; ++k) {
        uint8_t dest = next_state[0][k];
        cur[dest] += (seq_t)1;
    }

    /* --- process remaining rows --- */
    for (idx_t row = 2; row <= N; ++row) {
        /* zero next array */
        for (int i = 0; i < 256; ++i) nxt[i] = 0;

        /* propagate from current state array to next */
        for (int s = 0; s < 256; ++s) {
            seq_t cnt = cur[s];
            if (cnt == 0) continue;
            uint8_t len = next_len[s];
            for (uint8_t k = 0; k < len; ++k) {
                uint8_t dest = next_state[s][k];
                nxt[dest] += cnt;
            }
        }

        /* swap pointers for next iteration */
        seq_t *tmp_ptr = cur;
        cur = nxt;
        nxt = tmp_ptr;
    }

    /* --- sum all final states to obtain the answer --- */
    seq_t ans = 0;
    for (int i = 0; i < 256; ++i) ans += cur[i];
    return ans;
}
