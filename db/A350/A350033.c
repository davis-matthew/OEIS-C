/*
 * A350033  a(n) is the smallest positive integer k not occurring earlier such
 *          that Möbius(k) == n (mod 3).
 * https://oeis.org/A350033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 234, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the smallest unused positive integer whose Möbius
   value equals the pattern 1, -1, 0, 1, -1, 0, … (mod 3) repeated for
   indices n = 1, 2, 3, … This routine builds the sequence on demand
   and caches the results. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mobius.h"

/* Forward declaration of the helper function supplied by the build */
extern seq_t mobius(idx_t m);

/* ----------  state  ---------- */
static seq_t *seq = NULL;          /* array of returned terms */
static size_t seq_cap = 0;         /* capacity of seq */
static size_t seq_len = 0;         /* number of terms filled (|seq|) */

/* used[k] == 1 iff integer k already appears in seq */
static uint8_t *used = NULL;
static size_t used_cap = 0;        /* capacity of used array */

/* last_cand[3] – last candidate examined for each Möbius residue:
   0 → Möbius = 1
   1 → Möbius = -1
   2 → Möbius = 0 */
static size_t last_cand[3] = {0, 0, 0};

/* ----------  helpers  ---------- */
static void ensure_used(size_t n)
{
    if (n <= used_cap) return;
    size_t new_cap = used_cap ? used_cap << 1 : 128;
    while (new_cap < n) new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(used, new_cap * sizeof(uint8_t));
    if (!tmp) abort();
    memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(uint8_t));
    used = tmp;
    used_cap = new_cap;
}

static void ensure_seq(size_t n)
{
    if (n <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 128;
    while (new_cap < n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = new_cap;
}

static void init_once(void)
{
    if (seq) return;          /* already initialised */

    seq = (seq_t *)malloc(128 * sizeof(seq_t));
    if (!seq) abort();
    seq_cap = 128;
    seq_len = 0;

    used = (uint8_t *)malloc(128 * sizeof(uint8_t));
    if (!used) abort();
    used_cap = 128;
    memset(used, 0, 128 * sizeof(uint8_t));

    last_cand[0] = last_cand[1] = last_cand[2] = 0;
}

/* ----------  the required function  ---------- */
seq_t a(idx_t n)
{
    init_once();

    /* Build the sequence up to index n (1‑based) */
    while (seq_len < (size_t)n) {
        size_t idx = seq_len + 1;             /* next index */
        size_t res = (idx - 1) % 3;           /* required Möbius residue */

        size_t cand = last_cand[res] + 1;     /* start searching after last */
        for (;;) {
            ensure_used(cand + 1);            /* make sure index cand is valid */

            if (!used[cand]) {               /* candidate not seen before */
                seq_t mu = mobius((idx_t)cand);
                bool matches = false;
                if (res == 0) matches = (mu == (seq_t)1);
                else if (res == 1) matches = (mu == (seq_t)-1);
                else matches = (mu == (seq_t)0);

                if (matches) {
                    /* record the term */
                    ensure_seq(seq_len + 1);
                    seq[seq_len++] = (seq_t)cand;
                    used[cand] = 1;
                    last_cand[res] = cand;
                    break;
                }
            }
            ++cand;                      /* try next integer */
        }
    }

    return seq[n - 1];                    /* 0‑based array access */
}
