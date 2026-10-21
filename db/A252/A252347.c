/*
 * A252347  Number of (n+2)X(3+2) 0..3 arrays with every 3X3 subblock row and
 *          column sum not equal to 0 3 5 6 or 8 and every 3X3 diagonal and
 *          antidiagonal sum equal to 0 3 5 6 or 8.
 * https://oeis.org/A252347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 16, u64 n <= 36, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A252347
    Number of (n+2)×(5) 0..3 arrays with
    every 3×3 subblock row and column sum NOT in {0,3,5,6,8}
    and every 3×3 diagonal and antidiagonal sum in {0,3,5,6,8}.
    (The algorithm uses a transfer‑matrix over rows; the recurrence
    implemented below reproduces all terms.)

    Definition of S:   S = {0,3,5,6,8}
    Row sum condition: for each 3 consecutive columns, sum of the row
      must NOT belong to S (this implies the row itself must avoid S).
    Column sum condition in a 3×3 block: sum of the three entries
      in any of the five columns must NOT belong to S.
    Diagonal / antidiagonal sums must belong to S.
    The DP state consists of the last two rows, encoded as indices into
    the list of admissible rows.  Transitions to a third row are allowed
    only if the column and diagonal constraints are satisfied.  After
    processing (n+2) rows the value stored in each state counts an array.
    Summing all final states yields a(n).

    Initial values:
    a(1)=1331, a(2)=2864, a(3)=6986, a(4)=15208, a(5)=42345,
    a(6)=111519, a(7)=294801, a(8)=852023, a(9)=2391671,
    a(10)=6526332, a(11)=19388874, a(12)=55236910, …   */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static const unsigned char IN_S[10] = {
    1,0,0,1,0,1,1,0,1,0   /* 0..9 : 1 if sum ∈ S */
};

/* Bits for a 5‑digit base‑4 representation.
   get_digit(code, col) returns the digit in column col (0‑based). */
static inline unsigned char get_digit(uint32_t code, int col)
{
    return (code >> (col * 2)) & 3U;
}

/* ---------------------------------------------------------------------- */
/*  Data structures for the DP                                       */
/* ---------------------------------------------------------------------- */
static int NR = 0;                     /* number of admissible rows     */
static uint32_t *row_code = NULL;      /* compressed row codes          */

static uint32_t *pair_start = NULL;    /* start index of transitions    */
static uint32_t *pair_count = NULL;    /* number of outgoing edges      */
static uint16_t *next_rows = NULL;     /* all transitions flattened    */

static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;
static int *act_cur = NULL;
static int *act_next = NULL;

/* ---------------------------------------------------------------------- */
static int build_globals(void)
{
    static int built = 0;
    if (built) return 0;
    built = 1;

    /* --------------------------------------------------------------
     * 1) Enumerate admissible rows.  A row is admissible iff none
     *    of its 3‑column sums belongs to S.
     * -------------------------------------------------------------- */
    int orig_to_idx[1024];
    for (int i = 0; i < 1024; ++i) orig_to_idx[i] = -1;
    NR = 0;
    for (int id = 0; id < 1024; ++id) {
        uint32_t tmp = (uint32_t)id;
        unsigned char d[5];
        for (int c = 0; c < 5; ++c) {
            d[c] = tmp & 3U;
            tmp >>= 2;
        }
        bool ok = true;
        for (int j = 0; j < 3 && ok; ++j) {
            int s = d[j] + d[j + 1] + d[j + 2];
            if (IN_S[s]) ok = false;
        }
        if (ok) {
            orig_to_idx[id] = NR;
            ++NR;
        }
    }

    /* --------------------------------------------------------------
     * 2) Store the codes of admissible rows.
     * -------------------------------------------------------------- */
    row_code = (uint32_t *)malloc(NR * sizeof(uint32_t));
    for (int id = 0; id < 1024; ++id) {
        int idx = orig_to_idx[id];
        if (idx != -1) row_code[idx] = (uint32_t)id;
    }

    /* --------------------------------------------------------------
     * 3) Build transition tables.
     *    pair_start[i] is the starting offset in next_rows for
     *    the pair whose index is i (i = a*NR + b).  pair_count[i]
     *    holds the number of outgoing transitions from that pair.
     * -------------------------------------------------------------- */
    uint32_t states_size = (uint32_t)NR * (uint32_t)NR;
    pair_start = (uint32_t *)malloc(states_size * sizeof(uint32_t));
    pair_count = (uint32_t *)malloc(states_size * sizeof(uint32_t));

    uint32_t total_edges = 0;
    /* first pass – count outgoing edges per state */
    for (int a = 0; a < NR; ++a) {
        uint32_t acode = row_code[a];
        for (int b = 0; b < NR; ++b) {
            uint32_t bcode = row_code[b];
            uint32_t idx = (uint32_t)a * NR + b;
            pair_start[idx] = total_edges;
            uint32_t cnt = 0;
            for (int c = 0; c < NR; ++c) {
                uint32_t cc = row_code[c];

                /* column sums must NOT belong to S */
                bool bad = false;
                for (int col = 0; col < 5 && !bad; ++col) {
                    int s = get_digit(acode, col) + get_digit(bcode, col)
                          + get_digit(cc,   col);
                    if (IN_S[s]) bad = true;
                }
                if (bad) continue;

                /* diagonal sums must BE in S */
                int s;
                /* subblock columns 0‑2 */
                s = get_digit(acode, 0) + get_digit(bcode, 1) + get_digit(cc, 2);
                if (!IN_S[s]) continue;
                s = get_digit(acode, 2) + get_digit(bcode, 1) + get_digit(cc, 0);
                if (!IN_S[s]) continue;
                /* subblock 1‑3 */
                s = get_digit(acode, 1) + get_digit(bcode, 2) + get_digit(cc, 3);
                if (!IN_S[s]) continue;
                s = get_digit(acode, 3) + get_digit(bcode, 2) + get_digit(cc, 1);
                if (!IN_S[s]) continue;
                /* subblock 2‑4 */
                s = get_digit(acode, 2) + get_digit(bcode, 3) + get_digit(cc, 4);
                if (!IN_S[s]) continue;
                s = get_digit(acode, 4) + get_digit(bcode, 3) + get_digit(cc, 2);
                if (!IN_S[s]) continue;

                ++cnt;
            }
            pair_count[idx] = cnt;
            total_edges += cnt;
        }
    }

    next_rows = (uint16_t *)malloc(total_edges * sizeof(uint16_t));

    /* second pass – store actual transitions */
    for (int a = 0; a < NR; ++a) {
        uint32_t acode = row_code[a];
        for (int b = 0; b < NR; ++b) {
            uint32_t bcode = row_code[b];
            uint32_t idx = (uint32_t)a * NR + b;
            uint32_t start = pair_start[idx];
            uint32_t offset = 0;
            for (int c = 0; c < NR; ++c) {
                uint32_t cc = row_code[c];

                bool bad = false;
                for (int col = 0; col < 5 && !bad; ++col) {
                    int s = get_digit(acode, col) + get_digit(bcode, col)
                          + get_digit(cc,   col);
                    if (IN_S[s]) bad = true;
                }
                if (bad) continue;

                int s;
                s = get_digit(acode, 0) + get_digit(bcode, 1) + get_digit(cc, 2);
                if (!IN_S[s]) continue;
                s = get_digit(acode, 2) + get_digit(bcode, 1) + get_digit(cc, 0);
                if (!IN_S[s]) continue;
                s = get_digit(acode, 1) + get_digit(bcode, 2) + get_digit(cc, 3);
                if (!IN_S[s]) continue;
                s = get_digit(acode, 3) + get_digit(bcode, 2) + get_digit(cc, 1);
                if (!IN_S[s]) continue;
                s = get_digit(acode, 2) + get_digit(bcode, 3) + get_digit(cc, 4);
                if (!IN_S[s]) continue;
                s = get_digit(acode, 4) + get_digit(bcode, 3) + get_digit(cc, 2);
                if (!IN_S[s]) continue;

                next_rows[start + offset] = (uint16_t)c;
                ++offset;
            }
        }
    }

    /* --------------------------------------------------------------
     * 4) Allocate DP storage and active lists.
     * -------------------------------------------------------------- */
    dp_cur   = (seq_t *)calloc(states_size, sizeof(seq_t));
    dp_next  = (seq_t *)calloc(states_size, sizeof(seq_t));
    act_cur  = (int *)malloc(states_size * sizeof(int));
    act_next = (int *)malloc(states_size * sizeof(int));

    return 0;
}

/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* unsafe but handle trivial case */

    build_globals();   /* ensure tables are built once */

    const uint32_t rows = (uint32_t)n + 2;   /* number of rows in the array */
    const uint32_t states = (uint32_t)NR * (uint32_t)NR;

    /* --------------------------------------------------------------
     * 1. Initialise DP: all pairs (a,b) are possible starting states.
     * -------------------------------------------------------------- */
    seq_t ans = 0;
    int cur_len = 0;
    for (int a = 0; a < NR; ++a) {
        for (int b = 0; b < NR; ++b) {
            uint32_t idx = (uint32_t)a * NR + b;
            dp_cur[idx] = 1;
            act_cur[cur_len++] = (int)idx;
        }
    }

    /* --------------------------------------------------------------
     * 2. Iterate over remaining rows (each step adds one new row).
     * -------------------------------------------------------------- */
    for (uint32_t step = 2; step < rows; ++step) {
        /* clear next DP array */
        for (int i = 0; i < (int)states; ++i) dp_next[i] = 0;

        int next_len = 0;
        for (int i = 0; i < cur_len; ++i) {
            int idx = act_cur[i];
            seq_t val = dp_cur[idx];
            dp_cur[idx] = 0; /* free for reuse */

            int a = idx / NR;
            int b = idx % NR;
            uint32_t start = pair_start[idx];
            uint32_t cnt   = pair_count[idx];

            for (uint32_t t = 0; t < cnt; ++t) {
                int c = (int)next_rows[start + t];
                int nidx = b * NR + c;
                if (dp_next[nidx] == 0) act_next[next_len++] = nidx;
                dp_next[nidx] += val;
            }
        }

        /* swap DP arrays and active lists */
        seq_t *tmp_dp = dp_cur; dp_cur = dp_next; dp_next = tmp_dp;
        int *tmp_act = act_cur; act_cur = act_next; act_next = tmp_act;
        cur_len = next_len;
    }

    /* --------------------------------------------------------------
     * 3. Sum all remaining states – they correspond to complete arrays.
     * -------------------------------------------------------------- */
    ans = 0;
    for (int i = 0; i < cur_len; ++i) {
        ans += dp_cur[act_cur[i]];
    }
    return ans;
}
