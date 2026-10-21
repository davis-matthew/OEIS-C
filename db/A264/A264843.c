/*
 * A264843  Maximal numbers of consecutive positive integers congruent to 1
 *          modulo 3 that are all squarefree.
 * https://oeis.org/A264843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the nth consecutive run of squarefree numbers
 *            of the form 3k+1 (k ≥ 0)
 * Range:     u8 n <= 22, u16 n <= 6715, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* forward declaration of the sequence function */
seq_t a(idx_t n);

/* ------------------------------------------------------------------
 *  Helpers for computing squarefreeness
 * ------------------------------------------------------------------ */

static uint32_t *spf = NULL;           /* smallest prime factor array   */
static size_t    spf_limit = 0;       /* current size of spf array      */

/* Build spf array up to LIMIT (exclusive).  The array is filled only
   once; subsequent calls are ignored.                                    */
static void init_spf(void)
{
    if (spf) return;                     /* already built */

    const size_t LIMIT = 2000000;        /* should cover all needed numbers for n ≤ 10000   */
    spf = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    if (!spf) abort();

    spf_limit = LIMIT;
    for (size_t i = 0; i <= LIMIT; ++i) spf[i] = 0;

    for (size_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= LIMIT) {
                for (size_t j = i * i; j <= LIMIT; j += i) {
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }
        }
    }
}

/* Return true iff value x (≤spf_limit) is square‑free.           */
static bool is_squarefree(seq_t x)
{
    uint64_t v = (uint64_t)x;            /* x is always small enough to fit in 64‑bit */

    while (v > 1) {
        uint64_t p = spf[(size_t)v];
        if (p == 0) p = v;              /* safety: treat as prime if not in spf      */
        uint8_t count = 0;
        while (v % p == 0) {
            v /= p;
            ++count;
            if (count > 1) return false; /* found a square factor */
        }
    }
    return true;
}

/* ------------------------------------------------------------------
 *  Block bookkeeping
 * ------------------------------------------------------------------ */

static seq_t *blocks = NULL;            /* dynamic array holding run lengths       */
static idx_t  block_count = 0;          /* number of stored blocks                 */
static idx_t  k_cursor = 0;             /* next k to process (k starts at 0)       */
static seq_t  cur_run   = 0;            /* current run length being accumulated   */
static size_t block_cap = 0;            /* capacity of the blocks array            */

/* Ensure blocks array can hold at least N elements.   */
static void grow_blocks(idx_t N)
{
    if (block_cap >= (size_t)N) return;
    size_t new_cap = (block_cap == 0) ? 32 : block_cap * 2;
    while (new_cap < (size_t)N) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(blocks, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    blocks     = tmp;
    block_cap  = new_cap;
}

/* Generate blocks until we have at least TARGET blocks stored. */
static void ensure_blocks(idx_t TARGET)
{
    if (block_count >= TARGET) return;

    init_spf();               /* make sure spf is ready */

    while (block_count < TARGET) {
        seq_t num = (seq_t)3 * k_cursor + 1;          /* 3k + 1 */

        if (is_squarefree(num))
            cur_run += 1;
        else {
            if (cur_run > 0) {
                grow_blocks(block_count + 1);
                blocks[block_count++] = cur_run;
                cur_run = 0;
            }
        }
        ++k_cursor;
    }
}

/* ------------------------------------------------------------------
 *  Sequence function
 * ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* undefined for n=0, defensive */
    ensure_blocks(n);
    return blocks[n - 1];
}
