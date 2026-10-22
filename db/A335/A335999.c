/*
 * A335999  a(1) = 1; for n >= 2, a(n) = least positive integer not in
 *          {a(1),..., a(n-1), b(1),...,b(n-1)}, where for n >=1, b(n) = n + 2
 *          + least positive integer not in {a(1),..., a(n-1), a(n),
 *          b(1),...,b(n-1)}.
 * https://oeis.org/A335999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n >= 2, a(n) is the least positive integer not in
 *            {a(1),...,a(n-1), b(1),...,b(n-1)}, b(n) = n + 2 + least
 *            positive integer not in {a(1),...,a(n-1), a(n),
 *            b(1),...,b(n-1)}. The sequences a(n) and b(n) are complementary
 *            sets
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *a_arr      = NULL;   /* cache of a(n) values   */
static size_t a_cap      = 0;      /* capacity of a_arr      */
static idx_t   a_sz      = 0;      /* number of a terms cached */

static unsigned char *used   = NULL;  /* bitmap of used indices */
static idx_t         used_cap = 0;    /* capacity of used array */

static idx_t a_next = 1;  /* candidate for the next a(n) */

/* Ensure used array covers indices up to 'need'. */
static void ensure_used_capacity(idx_t need)
{
    if (used_cap <= need) {
        size_t new_cap = used_cap > 0 ? used_cap : 1;
        while (new_cap <= (size_t)need)
            new_cap <<= 1;
        unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
        if (!tmp) exit(EXIT_FAILURE);
        if (new_cap > used_cap)
            memset(tmp + used_cap, 0, new_cap - used_cap);
        used = tmp;
        used_cap = new_cap;
    }
}

/* Ensure a_arr can hold at least one more element. */
static void ensure_a_capacity(void)
{
    if (a_cap > a_sz) return;
    size_t new_cap = a_cap > 0 ? a_cap << 1 : 4;
    seq_t *tmp = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    a_arr = tmp;
    a_cap = new_cap;
}

/* Return a(n) for n >= 1.  Memoises all values up to the maximum requested. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    /* Compute terms up to n. */
    while (a_sz < n) {
        /* Find smallest unused integer -> a(n). */
        idx_t t = a_next;
        while (1) {
            ensure_used_capacity(t);
            if (!used[t]) break;
            ++t;
        }
        seq_t a_val = (seq_t)t;
        used[t] = 1;                     /* mark as used */

        /* Find smallest unused integer after a(n) -> auxiliary value for b(n). */
        idx_t u = t + 1;
        while (1) {
            ensure_used_capacity(u);
            if (!used[u]) break;
            ++u;
        }

        /* Compute b(n). */
        seq_t b_val = (seq_t)n + (seq_t)2 + (seq_t)u;
        ensure_used_capacity((idx_t)b_val);
        used[(idx_t)b_val] = 1;           /* mark as used */

        /* Cache a(n). */
        ensure_a_capacity();
        a_arr[a_sz] = a_val;

        /* Prepare for next iteration. */
        a_next = t + 1;
        ++a_sz;
    }

    return a_arr[n - 1];
}
