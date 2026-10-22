/*
 * A340612  a(0) = 0; for n > 0, if n appears in the sequence then a(n) =
 *          lastindex(n), where lastindex(n) is the index of the last
 *          appearance of n. Otherwise a(n) = a(n-1) - n if nonnegative and
 *          not already in the sequence, otherwise a(n) = a(n-1) + n.
 * https://oeis.org/A340612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0. For n>0,
 * if n has appeared before in the sequence,
 *    a(n)=lastindex(n)
 * otherwise
 *    a(n)=a(n-1)-n   if a(n-1)-n ≥0 and not already in the sequence,
 *    a(n)=a(n-1)+n   otherwise.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- value → last index map ---------- */
typedef struct { seq_t val; idx_t last; } ValEntry;
static ValEntry *val_map = NULL;
static size_t val_map_len = 0;
static size_t val_map_cap = 0;

static void ensure_val_map_cap(size_t needed)
{
    if (needed <= val_map_cap) return;
    size_t new_cap = val_map_cap < 16 ? 16 : val_map_cap * 2;
    while (new_cap < needed) new_cap *= 2;
    ValEntry *tmp = (ValEntry *)realloc(val_map, new_cap * sizeof(ValEntry));
    val_map = tmp;
    val_map_cap = new_cap;
}

static ValEntry *find_val_entry(seq_t v)
{
    for (size_t i = 0; i < val_map_len; ++i)
        if (val_map[i].val == v)
            return &val_map[i];
    return NULL;
}

static void upsert_val_entry(seq_t v, idx_t idx)
{
    ValEntry *e = find_val_entry(v);
    if (e) {
        e->last = idx;                /* update last occurrence */
    } else {
        ensure_val_map_cap(val_map_len + 1);
        val_map[val_map_len].val = v;
        val_map[val_map_len].last = idx;
        ++val_map_len;
    }
}

/* ---------- sequence storage ---------- */
static seq_t *seq = NULL;          /* array of sequence terms */
static idx_t seq_cap = 0;          /* allocated capacity */
static idx_t seq_len = 0;          /* number of terms stored */

static void ensure_seq_cap(idx_t needed)
{
    if (needed < seq_cap) return;   /* already large enough */
    idx_t new_cap = seq_cap < 16 ? 16 : seq_cap * 2;
    while (new_cap <= needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq = tmp;
    seq_cap = new_cap;
}

static void init_sequence(void)
{
    if (seq_len > 0) return;        /* already initialized */
    ensure_seq_cap(0);
    seq[0] = 0;
    seq_len = 1;
    upsert_val_entry(0, 0);
}

/* ---------- the required routine ---------- */
seq_t a(idx_t n)
{
    init_sequence();

    /* compute terms up to index n if they are missing */
    while (seq_len <= n) {
        idx_t i = seq_len;                /* current index to compute */
        ensure_seq_cap(i);                /* make sure seq has room */

        seq_t val;                        /* computed a(i) */

        /* rule: if i has appeared before, use its last index */
        ValEntry *e = find_val_entry((seq_t)i);
        if (e) {
            val = e->last;
        } else {
            /* otherwise try subtraction first if non‑negative */
            if (seq[i-1] >= (seq_t)i) {
                seq_t cand = seq[i-1] - (seq_t)i;
                if (!find_val_entry(cand))
                    val = cand;
                else
                    val = seq[i-1] + (seq_t)i;
            } else {
                /* subtraction would underflow – use addition */
                val = seq[i-1] + (seq_t)i;
            }
        }

        seq[i] = val;                     /* store the new term */
        upsert_val_entry(val, i);         /* update last‑index table */
        ++seq_len;
    }

    return seq[n];
}
