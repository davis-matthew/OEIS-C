/*
 * A334941  For any n > 0, let w be the least positive number such that the
 *          values (a(n+1-w), ..., a(n-1), e) do not appear continuously in
 *          (a(1), ..., a(n-1)) for some e in {0, 1}; a(n) is the least such
 *          e.
 * https://oeis.org/A334941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A334941 (minimal missing suffix)
 *
 *  For n > 0 let w be the smallest positive integer such that
 *  for some e ∈ {0,1} the pattern
 *      (a(n+1-w), a(n+2-w), …, a(n-1), e)
 *  does not occur as a contiguous block in (a(1), …, a(n-1)).
 *  Then a(n) is the least such e.
 *  The first 24 terms are
 *        0,1,0,0,0,1,1,0,1,0,1,1,1,0,0,1,0,0,1,1,1,1,0,1 …
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_W   20            /* maximal substring length required for n ≤ 10000 */

/* static data for memoisation */
static seq_t *seq = NULL;     /* 1‑based sequence array */
static size_t seq_cap = 0;    /* allocated size (1‑based) */
static size_t seq_len = 0;    /* number of values already computed */

static bool *seen[MAX_W + 1]; /* seen[w][mask] == true if pattern of length w was seen */
static size_t seen_cap[MAX_W + 1];

static uint64_t masks[MAX_W + 1]; /* last w bits ending at current position (0‑initialised) */

/* initialise static lookup tables once */
static void init(void)
{
    if (seen[1] != NULL) return;   /* already initialised */

    for (size_t w = 1; w <= MAX_W; ++w) {
        size_t sz = 1ULL << w;     /* number of possible patterns of length w */
        seen[w] = (bool*)calloc(sz, sizeof(bool));
        if (!seen[w]) abort();
        seen_cap[w] = sz;
    }
    /* masks are zero‑initialised automatically for static variables */
}

/* grow the sequence storage to hold at least n+1 elements (1‑indexed) */
static void grow_seq_storage(size_t n)
{
    if (seq_cap >= n) return;

    size_t new_cap = n * 2 + 10;          /* exponential growth */
    seq_t *tmp = (seq_t *)realloc(seq, (new_cap + 1) * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* append a new term to the sequence and update state */
static void append_next(void)
{
    size_t i = seq_len + 1;      /* new index to compute */
    seq_t ai = 0;                /* value to store */

    /* 1. find minimal w and e */
    unsigned maxw = (i < MAX_W) ? (unsigned)i : MAX_W;
    bool found = false;
    for (unsigned w = 1; w <= maxw && !found; ++w) {
        uint64_t prev_mask = (w == 1) ? 0u : masks[w - 1];
        for (unsigned e = 0; e <= 1; ++e) {
            uint64_t cand_mask = (prev_mask << 1) | e;
            if (!seen[w][(size_t)cand_mask]) {
                ai = (seq_t)e;
                found = true;
                break;
            }
        }
    }
    if (!found) ai = (seq_t)0;   /* should never happen */

    seq[i] = ai;

    /* 2. update masks and seen arrays */
    uint64_t bit = (uint64_t)ai;
    for (int w = (int)MAX_W; w >= 1; --w) {
        uint64_t new_mask = ((masks[w - 1] << 1) | bit) & ((1ULL << w) - 1);
        masks[w] = new_mask;
        if (i >= (size_t)w) {
            seen[w][(size_t)new_mask] = true;
        }
    }

    seq_len = i;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n=0, return 0 for safety */

    init();

    /* ensure storage large enough and compute any missing terms */
    if ((size_t)n > seq_cap) grow_seq_storage((size_t)n);
    while (seq_len < (size_t)n) append_next();

    return seq[(size_t)n];
}
