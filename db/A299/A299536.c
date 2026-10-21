/*
 * A299536  Solution b( ) of the complementary equation a(n) = b(n-1) +
 *          b(n-3), where a(0) = 1, a(1) = 2, a(2) = 3; see Comments.
 * https://oeis.org/A299536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   ensure_arr_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) is the sequence of numbers b(n) defined by
   b(0)=least positive integer not in {a(0),a(1),a(2)} with
   a(0)=1, a(1)=2, a(2)=3.
   For n>1, b(n)=least positive integer not equal to any of
   a(0)…a(n) or b(0)…b(n-1).
   The auxiliary sequence a(i) is defined by a(i)=b(i-1)+b(i-3) for i>2.
   The values returned by a(idx_t n) are the b(n) values listed in the
   OEIS entry.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ensure_arr_capacity.h"

static size_t used_cap = 0;
static uint8_t *used_tab = NULL;

/* ensure used_tab can address value 'v' */
static void ensure_used_capacity(idx_t v)
{
    size_t req = (size_t)v + 1;
    if (req <= used_cap) return;
    size_t new_cap = used_cap ? used_cap << 1 : 64;
    while (new_cap < req) new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(used_tab, new_cap);
    if (!tmp) abort();
    memset(tmp + used_cap, 0, new_cap - used_cap);
    used_tab = tmp;
    used_cap = new_cap;
}

static void mark_used(seq_t value)
{
    idx_t v = (idx_t)value;
    ensure_used_capacity(v);
    used_tab[v] = 1;
}

static seq_t next_missing(void)
{
    static idx_t next = 1;
    for (;; ++next) {
        ensure_used_capacity(next);
        if (!used_tab[next]) {
            return (seq_t)next;
        }
    }
}

static size_t a_cap = 0, b_cap = 0;
static seq_t *a_arr = NULL, *b_arr = NULL;
static idx_t next_b_computed = 0;   /* smallest b index yet to compute */

static void init_sequences(void)
{
    /* a(0)=1, a(1)=2, a(2)=3 */
    ensure_arr_capacity(&a_arr, &a_cap, 2);
    a_arr[0] = (seq_t)1;
    a_arr[1] = (seq_t)2;
    a_arr[2] = (seq_t)3;
    mark_used(a_arr[0]);
    mark_used(a_arr[1]);
    mark_used(a_arr[2]);

    /* b(0..2) : least missing positive integers */
    ensure_arr_capacity(&b_arr, &b_cap, 2);
    for (idx_t i = 0; i <= 2; ++i) {
        seq_t val = next_missing();
        b_arr[i] = val;
        mark_used(val);
    }
    next_b_computed = 3;  /* indices 0,1,2 already computed */
}

seq_t a(idx_t n)
{
    if (next_b_computed == 0) init_sequences();

    if ((idx_t)n >= next_b_computed) {
        for (idx_t i = next_b_computed; i <= n; ++i) {
            /* compute a[i] for i >= 3 */
            if (i >= 3) {
                seq_t ai = b_arr[i - 1] + b_arr[i - 3];
                ensure_arr_capacity(&a_arr, &a_cap, i);
                a_arr[i] = ai;
                mark_used(ai);
            }
            /* compute b[i] as next unused integer */
            ensure_arr_capacity(&b_arr, &b_cap, i);
            seq_t bi = next_missing();
            b_arr[i] = bi;
            mark_used(bi);
        }
        next_b_computed = n + 1;
    }

    return b_arr[n];
}
