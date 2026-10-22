/*
 * A358608  a(n) = Sum_{k=0..floor(n/3)} (-1)^k * (n-3*k)!.
 * https://oeis.org/A358608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A358608
   a(n) = Σ_{k=0..⌊n/3⌋} (-1)^k * (n-3k)!
   Recurrence for n > 3:
       a(n) = n*a(n-1) - a(n-3) + n*a(n-4)
   Initial values:
       a(0)=1, a(1)=1, a(2)=2, a(3)=5
*/
#include <stdlib.h>
#include <string.h>

static seq_t *a_cache = NULL;    /* memoised values of a(n) */
static size_t a_cap   = 0;       /* allocated capacity of the cache */

/* ensure the cache holds indices up to n inclusive */
static void a_ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= a_cap)
        return;
    size_t newcap = (a_cap == 0) ? 4 : a_cap;
    while (newcap < needed)
        newcap <<= 1;          /* double until enough */
    seq_t *newbuf = (seq_t *)realloc(a_cache, newcap * sizeof(seq_t));
    if (!newbuf) {
        /* allocation failed – terminate program (sequence is required) */
        abort();
    }
    /* Initialize newly allocated tail to zero (uncomputed) */
    if (newcap > a_cap) {
        memset(newbuf + a_cap, 0, (newcap - a_cap) * sizeof(seq_t));
    }
    a_cache = newbuf;
    a_cap   = newcap;
}

seq_t a(idx_t n)
{
    /* Handle trivial small n directly */
    if (n <= 3) {
        switch ((idx_t)n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)1;
        case 2: return (seq_t)2;
        case 3: return (seq_t)5;
        default: return (seq_t)0;   /* unreachable */
        }
    }

    a_ensure_capacity(n);

    /* Initialize base values on first use */
    if (a_cache[0] == 0) {
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)1;
        a_cache[2] = (seq_t)2;
        a_cache[3] = (seq_t)5;
    }

    /* Compute values up to n if not already cached */
    for (idx_t i = 4; i <= n; ++i) {
        if (a_cache[i] == 0) {    /* not yet computed */
            seq_t term1 = (seq_t)i * a_cache[i-1];
            seq_t term2 = a_cache[i-3];
            seq_t term3 = (seq_t)i * a_cache[i-4];
            a_cache[i] = term1 - term2 + term3;
        }
    }

    return a_cache[n];
}
