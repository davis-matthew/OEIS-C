/*
 * A257256  Difference between {the first unlucky number removed at the n-th
 *          stage of Lucky sieve} and {the n-th Lucky number}: a(n) =
 *          A219178(n) - A000959(n).
 * https://oeis.org/A257256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A219178(n) - A000959(n)
 * Range:     u8 n <= 14, u16 n <= 767, u32 n <= 767, u64 n <= 767,
 *            u128 n <= 767, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = first unlucky number removed at stage n
   minus the n-th lucky number */

/* This implementation pre-computes the lucky sieve up to a fixed
   maximum number (INIT_CAP).  It keeps two buffers that are used
   alternately to hold the list of remaining numbers after each
   stage.  The difference for each stage is stored in a static
   cache so that subsequent calls with a larger n do not redo work. */

#include <stdlib.h>

#define INIT_CAP (2400000)   /* enough to cover n up to 10000 */

static seq_t *cur = NULL;          /* current list of remaining numbers   */
static seq_t *nxt = NULL;          /* temporary buffer for next stage     */
static size_t cur_len = 0;         /* length of current list              */
static idx_t  computed_stage = 0;  /* how many stages have been processed */
static seq_t *cache = NULL;        /* cache for a(1)...a(n)               */
static size_t cache_cap = 0;       /* capacity of cache array             */

/* Initialise the buffers and the initial list of numbers */
static void init_buffers(void)
{
    size_t i;
    cur = (seq_t *)malloc(INIT_CAP * sizeof(seq_t));
    nxt = (seq_t *)malloc(INIT_CAP * sizeof(seq_t));
    for (i = 0; i < INIT_CAP; ++i)
        cur[i] = (seq_t)(i + 1);    /* natural numbers starting at 1     */
    cur_len = INIT_CAP;

    /* initialise cache with capacity to hold at least one element */
    cache_cap = 16;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
}

/* Ensure cache can hold values up to index n (1‑based) */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1; /* +1 because index 0 is unused */
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Perform the representation of lucky sieve up to stage n */
static void compute_upto(idx_t n)
{
    size_t i, pos, step_idx;
    seq_t first_removed, lucky, step;
    size_t new_len;
    /* initialise if this is the first call */
    if (!cur) init_buffers();

    while (computed_stage < n) {
        ++computed_stage;                   /* stage number (1‑based)        */
        /* ---- determine step size ---- */
        if (computed_stage == 1)
            step = 2;                       /* stage 1 always removes evens  */
        else
            step = cur[computed_stage - 1]; /* step = current lucky number   */

        step_idx = (size_t)step;            /* position of first removed     */

        /* ---- record the first unlucky removed ---- */
        first_removed = cur[step_idx - 1];  /* value at that position       */

        /* ---- build next list, removing every step‑th element ---- */
        new_len = 0;
        for (pos = 0; pos < cur_len; ++pos) {
            if (((pos + 1) % step_idx) != 0) {
                nxt[new_len++] = cur[pos];
            }
        }

        /* swap buffers */
        { seq_t *tmp = cur; cur = nxt; nxt = tmp; }
        cur_len = new_len;

        /* ---- lucky number after this stage ---- */
        lucky = cur[computed_stage - 1];    /* first n numbers survive      */

        /* ---- store the difference ---- */
        ensure_cache(computed_stage);
        cache[computed_stage] = first_removed - lucky;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for 0, but safe return */
    compute_upto(n);
    return cache[n];
}
