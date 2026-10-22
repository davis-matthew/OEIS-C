/*
 * A327723  a(1) = 0. For n > 1, a(n) is the smallest positive integer k not
 *          already in the sequence such that the least significant digit of k
 *          equals the most significant digit of a(n-1).
 * https://oeis.org/A327723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   msd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A327723
 *
 *  a(1) = 0.
 *  For n > 1, a(n) is the smallest positive integer k not already
 *  in the sequence such that the least‑significant digit of k equals
 *  the most‑significant digit of a(n‑1).
 *  (The function builds the sequence on demand, so it works for any
 *  order of calls.)
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/msd_seq.h"

/*--- helpers --------------------------------------------------------*/

/*--- dynamic arrays ------------------------------------------------*/

static seq_t *seq  = NULL;       /* sequence values                */
static size_t seq_len = 0;       /* number of values computed      */
static size_t seq_cap = 0;       /* allocated capacity             */

static bool *used = NULL;        /* bitset of used numbers         */
static size_t used_cap = 0;      /* allocated capacity             */

static void ensure_seq_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2U : 16U;
    if (new_cap < needed) new_cap = needed;
    seq_t *new_seq = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_seq) abort();
    if (seq) {
        memcpy(new_seq, seq, seq_len * sizeof(seq_t));
        free(seq);
    }
    seq     = new_seq;
    seq_cap = new_cap;
}

static void ensure_used_capacity(uint64_t val)
{
    size_t needed = (size_t)val + 1U;
    if (needed <= used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2U : 1024U;
    while (new_cap < needed) new_cap *= 2U;
    bool *new_used = (bool*)calloc(new_cap, sizeof(bool));
    if (!new_used) abort();
    if (used) {
        memcpy(new_used, used, used_cap * sizeof(bool));
        free(used);
    }
    used     = new_used;
    used_cap = new_cap;
}

static bool is_used(uint64_t v)
{
    ensure_used_capacity(v);
    return used[v];
}

static void mark_used(uint64_t v)
{
    ensure_used_capacity(v);
    used[v] = true;
}

/*--- main sequence generator ---------------------------------------*/

static void generate_up_to(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        /* start sequence with a(1)=0 */
        ensure_seq_capacity(1U);
        seq[0] = (seq_t)0U;
        seq_len = 1U;
        mark_used(0U);
        initialized = true;
    }

    static uint64_t next_candidate[10] = {10U,1U,2U,3U,4U,5U,6U,7U,8U,9U};

    while (seq_len < (size_t)n) {
        seq_t prev = seq[seq_len - 1];
        uint64_t d  = msd_seq(prev);        /* most significant digit of prev */

        uint64_t k = next_candidate[d];
        while (is_used(k)) k += 10U;        /* find smallest unused ending in d */

        seq_t val = (seq_t)k;
        ensure_seq_capacity(seq_len + 1U);
        seq[seq_len] = val;
        mark_used(k);
        next_candidate[d] = k + 10U;        /* next possible candidate for digit d */

        seq_len++;
    }
}

/*--- public function ------------------------------------------------*/

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0U;   /* undefined for n<1, but harmless */
    generate_up_to(n);
    return seq[n - 1];
}
