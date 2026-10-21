/*
 * A295050  Number of nX7 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 0 or 2 1s.
 * https://oeis.org/A295050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A295050 via column‑by‑column DP over all feasible
   7‑row binary column patterns.  The transition graph is built once
   and reused for every call to a(idx_t).  The implementation works
   for the integer seq_t used by the build system, including the
   arbitrary‑precision class handled by the big integer build. */

/* ***** Definitions for the automaton ***** */
#define R          7                     /* number of rows in a column   */
#define PAT_BITS   (1 << R)              /* 128 possible column patterns*/
#define MAX_STATES (1 << (2 * R))        /* 16384 states (pattern,req)   */
#define MAX_TRANS  128                   /* conservative upper bound */

/* ***** Global tables ***** */
static int          state_cnt = 0;                /* number of states visited */
static uint8_t      state_pattern[MAX_STATES];    /* column pattern of a state*/
static uint8_t      state_req[MAX_STATES];        /* required 1 on right side   */
static int          trans_next[MAX_STATES][MAX_TRANS];
static uint8_t      trans_len[MAX_STATES];        /* number of outgoing edges   */
static int          built = 0;                    /* state graph built flag     */

/* ***** Build the transition graph (once) ***** */
static void build_states(void)
{
    if (built) return;
    built = 1;

    int state_index[MAX_STATES];
    for (int i = 0; i < MAX_STATES; ++i) state_index[i] = -1;

    int queue[MAX_STATES];
    int head = 0, tail = 0;

    /* initial state: all zeros, no pending right‑hand requirement */
    state_pattern[0] = 0;
    state_req[0]     = 0;
    state_index[0]   = 0;
    state_cnt        = 1;
    queue[tail++]    = 0;

    while (head < tail) {
        int s = queue[head++];
        uint8_t p_prev = state_pattern[s];
        uint8_t r_prev = state_req[s];

        uint8_t mask_one  = p_prev & r_prev;       /* prev bits that must be 1   */
        uint8_t mask_zero = p_prev & (~r_prev);     /* prev bits that must be 0   */

        for (uint8_t cur = 0; cur < PAT_BITS; ++cur) {
            /* satisfy left‑neighbor constraints from previous column */
            if ((cur & mask_one)  != mask_one)  continue;
            if ((cur & mask_zero) != 0)        continue;

            uint8_t next_req = 0;
            int      ok      = 1;

            /* evaluate 1‑cell degrees for current column */
            for (int i = 0; i < R && ok; ++i) {
                if ((cur >> i) & 1) {
                    int partial = 0;
                    /* left neighbour (previous column) */
                    if (p_prev & (1u << i)) partial++;
                    /* up neighbour (current column)   */
                    if (i > 0      && (cur & (1u << (i-1)))) partial++;
                    /* down neighbour (current column) */
                    if (i < R-1 && (cur & (1u << (i+1)))) partial++;
                    if (partial > 2) { ok = 0; break; }
                    if (partial == 1) next_req |= (1u << i);
                }
            }
            if (!ok) continue;

            unsigned int id_next = ((unsigned int)cur << R) | next_req;
            int t = state_index[id_next];
            if (t == -1) {                  /* new state discovery */
                t = state_cnt++;
                state_index[id_next] = t;
                state_pattern[t] = cur;
                state_req[t]     = next_req;
                queue[tail++] = t;
            }
            trans_next[s][trans_len[s]++] = t;   /* store transition */
        }
    }
}

/* ***** Main sequence function ***** */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* value outside of offset is irrelevant */

    build_states();                /* ensure transitions are ready */

    /* Allocate DP arrays; use calloc so they start zeroed (works for
       both integer and big‑int seq_t). */
    seq_t *curr = (seq_t *)calloc(state_cnt, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(state_cnt, sizeof(seq_t));
    curr[0] = 1;                    /* start from the empty left border */

    for (idx_t col = 1; col <= n; ++col) {
        memset(next, 0, state_cnt * sizeof(seq_t));
        for (int s = 0; s < state_cnt; ++s) {
            seq_t val = curr[s];
            if (val == 0) continue;
            int len = trans_len[s];
            for (int j = 0; j < len; ++j) {
                int t = trans_next[s][j];
                next[t] += val;
            }
        }
        seq_t *tmp = curr; curr = next; next = tmp;   /* advance stage */
    }

    /* Sum all states whose pending right‑hand requirement is 0. */
    seq_t result = 0;
    for (int s = 0; s < state_cnt; ++s) {
        if (state_req[s] == 0) result += curr[s];
    }

    free(curr);
    free(next);
    return result;
}
