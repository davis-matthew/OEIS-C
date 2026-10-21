/*
 * A296558  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n) - n, where a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 4, b(2) = 5,
 *          and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(0)=1, a(1)=3;
   a(n)=a(n-1)+a(n-2)+b(n)-n   for n≥2,
   where (a(n)) and (b(n)) are complementary increasing sequences
   with b(0)=2, b(1)=4, b(2)=5.
   This implementation builds a(n) on demand. */

#include <stdlib.h>
#include <stdbool.h>

struct seq_state_t {
    seq_t *a;          /* array of a-values */
    size_t a_cap;      /* capacity of a[]      */
    idx_t  done;       /* next index to compute */
    seq_t  last_b;     /* last b-value produced */
    size_t a_ptr;      /* helper index for finding next b */
};

static struct seq_state_t state = {0};

/* Ensure a[] can hold at least `needed` elements. */
static void ensure_a_capacity(size_t needed)
{
    if (needed <= state.a_cap) return;
    size_t new_cap = state.a_cap ? state.a_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(state.a, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    state.a = tmp;
    state.a_cap = new_cap;
}

/* Find the next unused integer to be b(n).  The helper `a_ptr` walks
   through the already computed a-values. */
static seq_t next_b(void)
{
    seq_t cand = state.last_b + 1;
    while (1) {
        while (state.a_ptr < state.done && state.a[state.a_ptr] < cand)
            state.a_ptr++;
        if (state.a_ptr < state.done && state.a[state.a_ptr] == cand) {
            cand++;                /* skip this a‑value */
            state.a_ptr++;         /* move past the skipped a‑value */
            continue;
        }
        state.last_b = cand;
        return cand;
    }
}

/* Initialise the global state once. */
static void state_init(void)
{
    state.a_cap  = 16;
    state.a      = (seq_t *)calloc(state.a_cap, sizeof(seq_t));
    if (!state.a) abort();

    /* Base terms */
    state.a[0] = (seq_t)1;
    state.a[1] = (seq_t)3;
    /* b(2)=5 is given */
    state.a[2] = state.a[0] + state.a[1] + (seq_t)5 - (seq_t)2;
    state.done   = 3;          /* indices 0..2 are ready */
    state.last_b = (seq_t)5;   /* b(2) used */
    state.a_ptr  = 0;          /* start of a[] for b search */
}

/* Public function */
seq_t a(idx_t n)
{
    if (!state.a) state_init();

    /* Compute a[n] if not yet cached. */
    while (state.done <= n) {
        idx_t i = state.done;

        seq_t bi = next_b();                      /* b(i)  */
        seq_t ai = state.a[i-1] + state.a[i-2] + bi - (seq_t)i;

        ensure_a_capacity((size_t)(i + 1));
        state.a[i] = ai;

        ++state.done;
    }
    return state.a[n];
}
