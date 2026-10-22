/*
 * A362093  a(1) = 1, a(2) = 2; for n > 2, a(n) is the least positive integer
 *          not occurring earlier such that a(n) shares no digit with a(n-1) -
 *          a(n-2).
 * https://oeis.org/A362093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2; for n>2, * a(n) = the least positive integer not
 *            already used * whose decimal digits share no digit with
 *            |a(n-1)-a(n-2)|
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static seq_t *seq_arr   = NULL;   /* cached terms, 0‑based: a(1)=seq_arr[0] */
static size_t seq_len   = 0;      /* number of terms stored          */
static size_t seq_cap   = 0;      /* capacity of seq_arr              */

static unsigned char *used_arr = NULL; /* bitmap for used values */
static size_t used_cap = 0;           /* capacity of used_arr */

static void ensure_seq_cap(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 1;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *p = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    if (!p) abort();
    seq_arr = p;
    seq_cap = new_cap;
}

static void ensure_used_cap(size_t needed)
{
    if (used_cap >= needed) return;
    size_t old_cap = used_cap;
    size_t new_cap = used_cap ? used_cap << 1 : 1;
    while (new_cap < needed) new_cap <<= 1;
    unsigned char *p = (unsigned char *)realloc(used_arr, new_cap);
    if (!p) abort();
    used_arr = p;
    used_cap = new_cap;
    memset(used_arr + old_cap, 0, new_cap - old_cap);
}

/* compute a bitmask of decimal digits present in v:
   bit d set iff digit d occurs in v */
static unsigned digit_mask(seq_t v)
{
    unsigned mask = 0;
    if (v == 0) return 1u << 0;
    while (v > 0) {
        unsigned d = (unsigned)(v % 10);
        mask |= 1u << d;
        v /= 10;
    }
    return mask;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined, but safe */
    size_t target = (size_t)n;

    /* already computed? */
    if (target <= seq_len) return seq_arr[target - 1];

    /* initialise first two terms on first call */
    if (seq_len == 0) {
        ensure_seq_cap(2);
        seq_arr[0] = (seq_t)1;
        seq_arr[1] = (seq_t)2;
        seq_len = 2;

        ensure_used_cap(3);                    /* need indices 0..2 */
        used_arr[1] = 1;
        used_arr[2] = 1;
    }

    /* generate terms up to 'target' */
    for (size_t i = seq_len; i < target; ++i) {
        seq_t prev      = seq_arr[i - 1];
        seq_t prevprev  = seq_arr[i - 2];
        seq_t diff = (prev >= prevprev) ? (prev - prevprev) : (prevprev - prev);
        unsigned diff_mask = digit_mask(diff);

        seq_t cand = 1;
        while (1) {
            if ((size_t)cand >= used_cap)
                ensure_used_cap((size_t)cand + 1);
            if (!used_arr[(size_t)cand] &&
                ((digit_mask(cand) & diff_mask) == 0))
                break;
            ++cand;
        }

        ensure_seq_cap(i + 1);
        seq_arr[i] = cand;
        seq_len = i + 1;

        if ((size_t)cand >= used_cap)
            ensure_used_cap((size_t)cand + 1);
        used_arr[(size_t)cand] = 1;
    }

    return seq_arr[target - 1];
}
