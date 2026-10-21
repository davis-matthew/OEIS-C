/*
 * A253539  Number of (n+2)X(3+2) 0..1 arrays with every 3X3 subblock diagonal
 *          maximum minus antidiagonal minimum nonincreasing horizontally and
 *          nondecreasing vertically.
 * https://oeis.org/A253539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of (n+2)×5 binary arrays whose 3×3 subblock
 *            diagonal‑max minus antidiagonal‑min values are horizontally
 *            nonincreasing and vertically nondecreasing. Computation is
 *            performed by a dynamic program over states (two previous rows
 *            and the values of the previous subblocks)
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 14, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define ROWS          32          /* 5-bit rows: 0 .. 31 */
#define STATE_D_BITS  3           /* d values (0/1) for 3 columns */
#define STATE_COUNT   (ROWS * ROWS * (1U << STATE_D_BITS))   /* 8192 */

/* Encode a state:
   rowA: higher row (oldest), rowB: newer row, d_bits: 3-bit vector of
   d values for the subblocks starting at the older row. */
static inline unsigned
encode(unsigned rowA, unsigned rowB, unsigned d_bits)
{
    return (rowA << 8) | (rowB << 3) | d_bits;
}

/* Decode a state into its components. */
static inline void
decode(unsigned state, unsigned *rowA, unsigned *rowB, unsigned *d_bits)
{
    *rowA  = state >> 8;
    *rowB  = (state >> 3) & 0x1F;
    *d_bits = state & 0x07;
}

/* Precomputed data for all 3‑row triples ------------------------------------------------------------------- */
static unsigned char triple_d_bits[ROWS][ROWS][ROWS];
static unsigned char triple_horiz[ROWS][ROWS][ROWS];
static bool          table_initialized = false;

static void init_triple_tables(void)
{
    if (table_initialized) return;
    for (unsigned a = 0; a < ROWS; ++a) {
        for (unsigned b = 0; b < ROWS; ++b) {
            for (unsigned c = 0; c < ROWS; ++c) {
                unsigned d_bits = 0;
                /* compute d[0..2] */
                for (int j = 0; j < 3; ++j) {
                    int a1 = (a >> j)       & 1;
                    int a2 = (b >> (j+1))   & 1;
                    int a3 = (c >> (j+2))   & 1;
                    int diagmax = a1;
                    if (a2 > diagmax) diagmax = a2;
                    if (a3 > diagmax) diagmax = a3;

                    int b1 = (a >> (j+2))   & 1;
                    int b2 = a2;            /* same as above */
                    int b3 = (c >> j)       & 1;
                    int antmin = b1;
                    if (b2 < antmin) antmin = b2;
                    if (b3 < antmin) antmin = b3;

                    int diff = diagmax - antmin;   /* 0 or 1 */
                    d_bits |= (diff & 1) << j;
                }
                /* store d_bits */
                triple_d_bits[a][b][c] = (unsigned char)d_bits;

                /* horizontal monotonicity check */
                bool horiz = true;
                bool d0 = (d_bits & 0x01) != 0;
                bool d1 = (d_bits & 0x02) != 0;
                bool d2 = (d_bits & 0x04) != 0;
                if (!((!d1 || d0) && (!d2 || d1))) horiz = false;
                triple_horiz[a][b][c] = (unsigned char)(horiz ? 1 : 0);
            }
        }
    }
    table_initialized = true;
}

/* Transition table for DP after the first triple -------------------------------------------- */
typedef struct {
    unsigned short count;                /* number of valid successors */
    unsigned short next[32];             /* successor state indices */
} Trans;

static Trans *trans_table = NULL;
static bool    trans_initialized = false;

static void init_transition_table(void)
{
    if (trans_initialized) return;
    trans_table = (Trans*)malloc(STATE_COUNT * sizeof(Trans));
    for (unsigned state = 0; state < STATE_COUNT; ++state) {
        trans_table[state].count = 0;
    }
    for (unsigned a = 0; a < ROWS; ++a) {
        for (unsigned b = 0; b < ROWS; ++b) {
            for (unsigned d_prev = 0; d_prev < (1U << STATE_D_BITS); ++d_prev) {
                unsigned state = encode(a, b, d_prev);
                Trans *t = &trans_table[state];
                for (unsigned c = 0; c < ROWS; ++c) {
                    unsigned d_bits = triple_d_bits[a][b][c];
                    if (!triple_horiz[a][b][c]) continue;          /* horizontal fails */

                    /* vertical monotonicity: d_prev <= d_bits per bit */
                    if ((d_prev & (~d_bits)) != 0) continue;       /* some bit 1 in d_prev but 0 in d_bits */

                    unsigned next_state = encode(b, c, d_bits);
                    t->next[t->count++] = (unsigned short)next_state;
                }
            }
        }
    }
    trans_initialized = true;
}

/* Main function to compute a(n) ------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Preconditions: n >= 1 (the offset is 1).  For n < 1 we simply return 0. */
    if (n < 1) return 0;

    init_triple_tables();
    init_transition_table();

    /* Number of total rows */
    idx_t rows = n + 2;
    /* After adding the first triple we have used 3 rows.
       Remaining rows to add = rows - 3. */
    idx_t remaining = rows - 3;
    /* Allocate DP arrays */
    seq_t *dp_cur = (seq_t*)calloc(STATE_COUNT, sizeof(seq_t));
    seq_t *dp_next = (seq_t*)calloc(STATE_COUNT, sizeof(seq_t));
    if (!dp_cur || !dp_next) {
        /* Memory error: return zero as per specification. */
        if (dp_cur) free(dp_cur);
        if (dp_next) free(dp_next);
        return 0;
    }

    /* ---------------------  Initial DP after the first 3 rows --------------------- */
    /* Enumerate all a,b,c and accumulate counts for each resulting state. */
    for (unsigned a = 0; a < ROWS; ++a) {
        for (unsigned b = 0; b < ROWS; ++b) {
            for (unsigned c = 0; c < ROWS; ++c) {
                if (!triple_horiz[a][b][c]) continue;
                unsigned d_bits = triple_d_bits[a][b][c];
                unsigned state = encode(b, c, d_bits);
                dp_cur[state] += (seq_t)1;
            }
        }
    }

    /* ---------------------  DP for remaining rows --------------------- */
    for (idx_t step = 0; step < remaining; ++step) {
        /* zero dp_next */
        for (size_t i = 0; i < STATE_COUNT; ++i) dp_next[i] = (seq_t)0;

        /* iterate over all states */
        for (unsigned s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0) continue;
            Trans *t = &trans_table[s];
            for (unsigned k = 0; k < t->count; ++k) {
                unsigned next = t->next[k];
                dp_next[next] += val;
            }
        }
        /* swap dp_cur and dp_next */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* Sum all remaining states to get the final answer */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < STATE_COUNT; ++i) {
        result += dp_cur[i];
    }

    /* Clean up */
    free(dp_cur);
    free(dp_next);

    return result;
}
