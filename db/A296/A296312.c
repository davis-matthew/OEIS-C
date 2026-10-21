/*
 * A296312  Number of nX7 0..1 arrays with each 1 adjacent to 3 or 6 king-move
 *          neighboring 1s.
 * https://oeis.org/A296312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of n×7 0..1 arrays with each 1 having 3 or 6 king‑move
 *            neighboring 1s. Computation uses dynamic programming over
 *            ordered pairs of consecutive rows. The function memoises all
 *            results for previous values of n
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#define MASK_BITS 7
#define MASK_MAX (1 << MASK_BITS)
#define STATE_BITS (MASK_BITS * 2)          /* 14 bits for a pair of rows   */
#define STATE_MAX  (1 << STATE_BITS)        /* 16384 states                 */

/* Transition list for a given (prev, cur) row pair */
typedef struct {
    uint16_t *data;   /* list of valid next row masks */
    size_t   len;     /* number of children            */
    size_t   cap;     /* capacity of data              */
} TransList;

/* Transition table indexed by ((prev<<7)|cur) */
static TransList trans[STATE_MAX];

/* ------------------------------------------------------------------ */
/* Check whether a 1 in the middle row has 3 or 6 king‑move neighbours */

static int is_valid(uint16_t pp, uint16_t p, uint16_t cur)
{
    uint16_t bits = p;
    while (bits) {
        int i = __builtin_ctz(bits);       /* index of least significant 1 bit   */
        bits &= bits - 1;                  /* clear that bit                     */
        int c = 0;

        /* neighbors from the row above */
        if (i > 0)      c += (pp >> (i - 1)) & 1;
        if (i < MASK_BITS-1) c += (pp >> (i + 1)) & 1;
        c += (pp >> i) & 1;

        /* neighbors from the same row (excluding the cell itself) */
        if (i > 0)      c += (p  >> (i - 1)) & 1;
        if (i < MASK_BITS-1) c += (p  >> (i + 1)) & 1;

        /* neighbors from the row below */
        if (i > 0)      c += (cur >> (i - 1)) & 1;
        if (i < MASK_BITS-1) c += (cur >> (i + 1)) & 1;
        c += (cur >> i) & 1;

        if (!(c == 3 || c == 6))
            return 0;                      /* invalid configuration             */
    }
    return 1;                              /* all 1‑cells satisfied              */
}

/* ------------------------------------------------------------------ */
/* Build all transition lists */

static void build_transitions(void)
{
    for (int pp = 0; pp < MASK_MAX; ++pp) {
        for (int p = 0; p < MASK_MAX; ++p) {
            TransList *t = &trans[(pp << MASK_BITS) | p];
            t->len = 0;
            t->cap = 0;
            t->data = NULL;

            for (int cur = 0; cur < MASK_MAX; ++cur) {
                if (is_valid((uint16_t)pp, (uint16_t)p, (uint16_t)cur)) {
                    if (t->len == t->cap) {
                        size_t newcap = (t->cap == 0) ? 4 : t->cap * 2;
                        t->data = (uint16_t *)realloc(t->data,
                                                      newcap * sizeof(uint16_t));
                        t->cap = newcap;
                    }
                    t->data[t->len++] = (uint16_t)cur;
                }
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* Dynamic‑programming state arrays and active lists */

/* Current and next DP count arrays (indexed by state id) */
static seq_t *curr = NULL, *nxt = NULL;

/* Active state buffers */
static int  act_buf[STATE_MAX];
static int  act_next_buf[STATE_MAX];
static int *act = act_buf;
static int *act_next = act_next_buf;
static size_t act_len = 0;

/* ------------------------------------------------------------------ */
/* Result memoisation */

static seq_t *memo   = NULL;
static size_t memo_len = 0;
static size_t memo_cap = 0;

/* ------------------------------------------------------------------ */
/* Internal initialisation */

static int initialized = 0;

static void init(void)
{
    curr = (seq_t *)calloc(STATE_MAX, sizeof(seq_t));
    nxt  = (seq_t *)calloc(STATE_MAX, sizeof(seq_t));
    if (!curr || !nxt)
        abort();

    build_transitions();

    /* Initialise first row: previous row is zero */
    act_len = 0;
    for (int m0 = 0; m0 < MASK_MAX; ++m0) {
        int id = m0;                 /* state id = (0<<7)|m0           */
        curr[id] = (seq_t)1;
        act[act_len++] = id;
    }

    /* Initialise memo array */
    memo_cap = 32;
    memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
    if (!memo)
        abort();

    /* Compute a(1):  the first row must be valid with the next row being zero */
    seq_t val = 0;
    for (size_t k = 0; k < act_len; ++k) {
        int id = act[k];
        int pp = id >> MASK_BITS;      /* always 0 here */
        int p  = id & (MASK_MAX - 1);  /* m0 */
        seq_t cnt = curr[id];
        if (cnt && is_valid((uint16_t)pp, (uint16_t)p, 0))
            val += cnt;
    }
    memo[0] = val;
    memo_len = 1;
    initialized = 1;
}

/* ------------------------------------------------------------------ */
/* Perform one DP step (advance from row r to row r+1) */

static void dp_step(void)
{
    /* zero next array */
    for (size_t i = 0; i < STATE_MAX; ++i)
        nxt[i] = (seq_t)0;

    size_t next_len = 0;

    for (size_t idx = 0; idx < act_len; ++idx) {
        int id  = act[idx];
        seq_t cnt = curr[id];
        if (cnt == (seq_t)0)
            continue;
        int p = id & (MASK_MAX-1);
        TransList *t = &trans[id];
        for (size_t j = 0; j < t->len; ++j) {
            int cur_mask = t->data[j];
            int nid  = (p << MASK_BITS) | cur_mask;
            if (nxt[nid] == (seq_t)0)
                act_next[next_len++] = nid;
            nxt[nid] += cnt;
        }
    }

    /* swap current and next arrays */
    seq_t *tmp = curr; curr = nxt; nxt = tmp;

    /* swap active lists */
    int *tmp_act = act; act = act_next; act_next = tmp_act;
    act_len = next_len;
}

/* ------------------------------------------------------------------ */
/* Compute the next term a(n+1) given that DP has been performed for n rows */

static seq_t compute_next(void)
{
    dp_step();
    seq_t val = 0;
    for (size_t k = 0; k < act_len; ++k) {
        int id = act[k];
        int pp = id >> MASK_BITS;
        int p  = id & (MASK_MAX - 1);
        seq_t cnt = curr[id];
        if (cnt && is_valid((uint16_t)pp, (uint16_t)p, 0))
            val += cnt;
    }
    return val;
}

/* ------------------------------------------------------------------ */
/* Public interface */

seq_t a(idx_t n)
{
    if (!initialized)
        init();

    if (n <= memo_len)
        return memo[n - 1];

    /* compute terms up to n */
    while (memo_len < n) {
        seq_t val = compute_next();

        if (memo_len == memo_cap) {
            memo_cap *= 2;
            memo = (seq_t *)realloc(memo, memo_cap * sizeof(seq_t));
            if (!memo)
                abort();
        }
        memo[memo_len++] = val;
    }
    return memo[n - 1];
}
