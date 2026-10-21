/*
 * A294477  Solution of the complementary equation a(n) = a(n-2) + b(n-1) + 2,
 *          where a(0) = 1, a(1) = 3, b(0) = 2.
 * https://oeis.org/A294477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + b(n-1) + 2, a(0)=1, a(1)=3, b(0)=2
 * Range:     u8 n <= 24, u16 n <= 485, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* --- static state -------------------------------------------------------- */
static idx_t   last_n_computed = 0;
static seq_t * a_seq = NULL;
static size_t  a_cap = 0;

static seq_t * b_seq = NULL;
static size_t  b_cap = 0;

/* bitset that marks numbers already occurring in either a or b      */
static unsigned char * used_bits  = NULL;
static idx_t           used_cap_val = 0;      /* largest value handled   */
static idx_t           next_unused  = 1;      /* next candidate for b   */

/* ----------------------------------------------------------------------- */
static void ensure_a_capacity(idx_t idx)
{
    if (a_cap > idx) return;
    idx_t new_cap = a_cap ? a_cap * 2 : 16;
    while (new_cap <= idx) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(a_seq, new_cap * sizeof(seq_t));
    if (!tmp) return;          /* allocation failure – program will abort soon   */
    a_seq = tmp;
    a_cap = new_cap;
}

static void ensure_b_capacity(idx_t idx)
{
    if (b_cap > idx) return;
    idx_t new_cap = b_cap ? b_cap * 2 : 16;
    while (new_cap <= idx) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(b_seq, new_cap * sizeof(seq_t));
    if (!tmp) return;
    b_seq = tmp;
    b_cap = new_cap;
}

/* enlarge the bitset so that value 'val' is addressable                 */
static void ensure_used_capacity(idx_t val)
{
    if (val <= used_cap_val) return;
    size_t new_bytes = (size_t)(val >> 3) + 1;
    unsigned char *tmp = (unsigned char *)realloc(used_bits, new_bytes);
    if (!tmp) return;       /* allocation failure – abort on use            */
    if (used_bits) {
        size_t old_bytes = (size_t)(used_cap_val >> 3) + 1;
        memset(tmp + old_bytes, 0, new_bytes - old_bytes);
    } else {
        memset(tmp, 0, new_bytes);
    }
    used_bits  = tmp;
    used_cap_val = val;
}

/* mark a value as used                                               */
static void mark_used(idx_t val)
{
    ensure_used_capacity(val);
    used_bits[val >> 3] |= (1u << (val & 7));
}

/* return the least positive integer that has not yet appeared in a
   or b, and mark it as used                                         */
static seq_t next_unused_value(void)
{
    idx_t val = next_unused;
    while (1) {
        ensure_used_capacity(val);
        if (!(used_bits[val >> 3] & (1u << (val & 7)))) {
            used_bits[val >> 3] |= (1u << (val & 7));
            next_unused = val + 1;
            return (seq_t)val;
        }
        ++val;
    }
}

/* initialise global state on first call                            */
static void init_sequence(void)
{
    if (a_seq) return;    /* already initialised */

    /* a(0)=1, a(1)=3 */
    ensure_a_capacity(1);
    a_seq[0] = (seq_t)1;
    a_seq[1] = (seq_t)3;

    /* b(0)=2 */
    ensure_b_capacity(0);
    b_seq[0] = (seq_t)2;

    /* mark used numbers 1,2,3 */
    mark_used(1);
    mark_used(2);
    mark_used(3);

    next_unused      = 1;
    last_n_computed  = 1;
}

/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_sequence();

    if (n <= last_n_computed) {
        return a_seq[n];
    }

    for (idx_t k = last_n_computed + 1; k <= n; ++k) {
        idx_t idx_b = k - 1;
        ensure_b_capacity(idx_b);
        seq_t val_b = next_unused_value();
        b_seq[idx_b] = val_b;

        ensure_a_capacity(k);
        seq_t val_a = a_seq[k - 2] + val_b + (seq_t)2;
        a_seq[k] = val_a;

        /* mark the newly generated a value as used */
        mark_used((idx_t)val_a);
    }

    last_n_computed = n;
    return a_seq[n];
}
