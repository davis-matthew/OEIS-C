/*
 * A333549  Consider the list (A333552) of numbers m defined by property that
 *          when the Recamán term A005132(m) is being computed, we are unable
 *          to subtract m from A005132(m-1) because, although A003132(m-1) >=
 *          m, the result of the subtraction, A005132(m-1)-m, is already in
 *          A005132; sequence gives the success...
 * https://oeis.org/A333549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 3440, u32 n <= 9197, u64 n <= 9197,
 *            u128 n <= 9197, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recamán collision sequence A333549
   Computed by simulating the Recamán sequence and recording collisions
   when the subtraction would produce a value already present.
   The a(n) terms are the values A005132(m-1)-m for the n-th collision. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* structure to hold computed collisions */
typedef struct {
    seq_t *arr;
    idx_t cap;
    idx_t len;
} seq_vec;

static seq_vec coll_vec = {NULL, 0, 0};
static unsigned char *seen = NULL;
static idx_t seen_cap = 0;
static idx_t next_step = 1;      /* next Recamán step number (m) */
static seq_t recam_prev = 0;     /* Recamán value at step m-1 */

/* ensure seen bitmap can address idx */
static void ensure_seen_cap(idx_t idx)
{
    if (idx < seen_cap) return;
    idx_t new_cap = (seen_cap == 0) ? 1 : seen_cap << 1;
    while (new_cap <= idx) new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(seen, (size_t)new_cap);
    if (!tmp) abort();
    if (new_cap > seen_cap)
        memset(tmp + (size_t)seen_cap, 0, (size_t)(new_cap - seen_cap));
    seen = tmp;
    seen_cap = new_cap;
}

/* ensure the collision array can store up to new_len elements */
static void ensure_coll_cap(idx_t new_len)
{
    if (new_len <= coll_vec.cap) return;
    idx_t new_cap = (coll_vec.cap == 0) ? 1 : coll_vec.cap << 1;
    while (new_cap <= new_len) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(coll_vec.arr, (size_t)(new_cap * sizeof(seq_t)));
    if (!tmp && new_cap > 0) abort();
    coll_vec.arr = tmp;
    coll_vec.cap = new_cap;
}

/* generate Recamán steps until we have at least need collisions */
static void generate_collisions(idx_t need)
{
    if (coll_vec.arr == NULL) {
        /* initial allocation for seen set, mark 0 as seen */
        seen_cap = 1;
        seen = (unsigned char *)calloc((size_t)seen_cap, 1);
        if (!seen) abort();
        seen[0] = 1;            /* a(0) = 0 is already in the set */
    }
    while (coll_vec.len < need) {
        idx_t m = next_step;            /* current step number m */
        bool can_subtract = recam_prev >= (seq_t)m;
        seq_t tent = 0;
        bool collision = false;
        if (can_subtract) {
            tent = recam_prev - (seq_t)m;   /* safe, no underflow */
            idx_t t_idx = (idx_t)tent;
            ensure_seen_cap(t_idx);
            if (seen[t_idx]) {
                /* collision: record the tentative value */
                collision = true;
                ensure_coll_cap(coll_vec.len + 1);
                coll_vec.arr[coll_vec.len++] = tent;
            }
        }
        seq_t next_val;
        if (can_subtract && !collision) {   /* subtract path viable and no collision */
            next_val = tent;
        } else {
            next_val = recam_prev + (seq_t)m;
        }
        /* mark the new Recamán value as seen */
        idx_t n_idx = (idx_t)next_val;
        ensure_seen_cap(n_idx);
        seen[n_idx] = 1;
        /* advance state */
        recam_prev = next_val;
        next_step   = m + 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined below offset, but safe */
    generate_collisions(n);
    return coll_vec.arr[n - 1];
}
