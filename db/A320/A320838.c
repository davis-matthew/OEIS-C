/*
 * A320838  a(0) = 0, a(n) is the number of x such that a(x) = a(n-1) and
 *          there exists no y such that x < y < n and a(y) > a(n-1).
 * https://oeis.org/A320838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0; for n > 0,
   a(n) = #{x : x < n, a(x) = a(n-1) and no y : x<y<n with a(y) > a(n-1)} */

#include <stdlib.h>

/* ---- Helper structures for memoisation ---- */
typedef struct {
    seq_t  val;       /* value key                           */
    idx_t *pos;       /* sorted list of positions of val     */
    size_t len;       /* number of elements in pos[]         */
    size_t cap;       /* allocated capacity of pos[]         */
} value_info;

static seq_t  *seq            = NULL;   /* sequence values                 */
static size_t seq_len          = 0;      /* number of stored terms           */
static size_t seq_cap          = 0;      /* allocated capacity               */

static idx_t  *prevGreater    = NULL;   /* previous greater index per term  */
static size_t pg_len           = 0;      /* unused, kept for symmetry        */
static size_t pg_cap           = 0;      /* unused, kept for symmetry        */

static idx_t  *stack          = NULL;   /* monotone decreasing stack         */
static size_t stack_len        = 0;      /* current stack size                */
static size_t stack_cap        = 0;      /* allocated capacity                 */

static value_info *values = NULL;      /* array of value_info               */
static size_t values_len = 0;          /* number of distinct values         */
static size_t values_cap = 0;          /* allocated capacity for values     */

/* ---- Capacity helpers ---- */
static void ensure_seq_capacity(size_t need)
{
    if (seq_cap >= need) return;
    size_t newcap = seq_cap ? seq_cap * 2 : 4;
    while (newcap < need) newcap <<= 1;
    seq            = (seq_t*)realloc(seq, newcap * sizeof(seq_t));
    prevGreater    = (idx_t*)realloc(prevGreater, newcap * sizeof(idx_t));
    seq_cap = newcap;
}

static void ensure_stack_capacity(size_t need)
{
    if (stack_cap >= need) return;
    size_t newcap = stack_cap ? stack_cap * 2 : 4;
    while (newcap < need) newcap <<= 1;
    stack = (idx_t*)realloc(stack, newcap * sizeof(idx_t));
    stack_cap = newcap;
}

static void ensure_value_positions_capacity(value_info *v, size_t need)
{
    if (v->cap >= need) return;
    size_t newcap = v->cap ? v->cap * 2 : 4;
    while (newcap < need) newcap <<= 1;
    v->pos = (idx_t*)realloc(v->pos, newcap * sizeof(idx_t));
    v->cap = newcap;
}

/* ---- Value indexing ---- */
static size_t find_value_index(seq_t v)
{
    for (size_t i = 0; i < values_len; ++i) {
        if (values[i].val == v) return i;
    }
    /* not found -> create new entry */
    if (values_len >= values_cap) {
        size_t newcap = values_cap ? values_cap * 2 : 4;
        values = (value_info*)realloc(values, newcap * sizeof(value_info));
        values_cap = newcap;
    }
    values[values_len].val = v;
    values[values_len].len = 0;
    values[values_len].cap = 4;
    values[values_len].pos = (idx_t*)malloc(values[values_len].cap * sizeof(idx_t));
    return values_len++;
}

static void add_position(seq_t v, idx_t pos)
{
    size_t idx = find_value_index(v);
    value_info *vi = &values[idx];
    if (vi->len >= vi->cap) ensure_value_positions_capacity(vi, vi->len + 1);
    vi->pos[vi->len++] = pos;
}

/* ---- Binary search: count elements <= bound ---- */
static size_t count_le(const value_info *vi, idx_t bound)
{
    size_t lo = 0, hi = vi->len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (vi->pos[mid] <= bound)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* ---- Main sequence function ---- */
seq_t a(idx_t n)
{
    /* ---------- Initialization for the first call ---------- */
    if (seq_len == 0) {
        /* a(0) = 0 */
        ensure_seq_capacity(1);
        seq[0]           = (seq_t)0;
        prevGreater[0]   = (idx_t)-1;     /* no previous greater */
        seq_len          = 1;

        /* initialise stack */
        stack          = (idx_t*)malloc(4 * sizeof(idx_t));
        stack_cap      = 4;
        stack_len      = 1;
        stack[0]       = 0;

        /* initialise value list */
        values_cap = 4;
        values = (value_info*)malloc(values_cap * sizeof(value_info));
        values_len = 0;
        add_position((seq_t)0, 0);
    }

    /* ---------- Ensure sequence computed up to n ---------- */
    while (seq_len <= n) {
        idx_t   i          = seq_len;           /* next index to compute */
        seq_t   prev_val   = seq[i - 1];        /* a(n-1) for this step  */
        idx_t   lastHigher = prevGreater[i - 1];/* previous greater of a(n-1) */

        /* Retrieve positions of the previous value */
        size_t idx_val = find_value_index(prev_val);
        const value_info *vPrev = &values[idx_val];

        /* Count occurrences of prev_val between lastHigher and i-1 inclusive */
        size_t countAll = count_le(vPrev, i - 1);
        size_t countPrev = lastHigher == (idx_t)-1 ? 0 : count_le(vPrev, lastHigher);
        seq_t cnt = (seq_t)(countAll - countPrev);  /* a(n) */

        /* Compute prevGreater for the new index i */
        while (stack_len > 0 && seq[stack[stack_len - 1]] <= cnt) {
            stack_len--;                           /* pop while not larger */
        }
        idx_t pg_i = (stack_len ? stack[stack_len - 1] : (idx_t)-1);

        /* Append new value and prevGreater */
        ensure_seq_capacity(seq_len + 1);
        seq[seq_len]          = cnt;
        prevGreater[seq_len]  = pg_i;
        seq_len++;

        /* Push new index onto stack for future prevGreater computations */
        ensure_stack_capacity(stack_len + 1);
        stack[stack_len++] = i;

        /* Record position i for value cnt */
        add_position(cnt, i);
    }

    return seq[n];
}
