/*
 * A273513  a(n) is the number of arithmetic triples n<p<q (three numbers in
 *          arithmetic progression) such that p and q contain no 2's in their
 *          ternary representation.
 * https://oeis.org/A273513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(0)=0, a(1)=0, a(2)=1; For n>=0: if n%3==0 or n%3==1:
 *            a(n)=a(n/3); if n%3==2: let m = n/3; if a(m+1)==0: a(n)=a(m)+1;
 *            else : a(n)=a(m)+a(m+1); The implementation caches previously
 *            computed values
 * Range:     u8 n <= 8874, u16 n <= 8874, u32 n <= 8874, u64 n <= 8874,
 *            u128 n <= 8874, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache   = NULL;      /* cache[n] holds a(n) */
static uint8_t *seen  = NULL;      /* seen[n]==1 if cache[n] valid */
static size_t  cap    = 0;         /* allocated size of cache and seen */

/* Ensure that cache and seen can hold indices up to n.  Allocate or grow
   the arrays as necessary, zero‑initialising any new slots. */
static void grow_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (cap >= need) return;

    size_t newcap = cap == 0 ? 8 : cap * 2;
    if (newcap < need) newcap = need;

    cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    seen  = (uint8_t *)realloc(seen,  newcap * sizeof(uint8_t));
    /* Mark new slots as unseen */
    for (size_t i = cap; i < newcap; ++i) seen[i] = 0;
    cap = newcap;
}

/* Recursive helper with memoisation. */
static seq_t rec(idx_t n)
{
    grow_cache(n);
    if (seen[n]) return cache[n];

    seq_t val;
    if (n == 0)        val = 0;
    else if (n == 1)   val = 0;
    else if (n == 2)   val = 1;
    else {
        idx_t rem = n % 3;
        if (rem == 0 || rem == 1) {
            val = rec(n / 3);
        } else { /* rem == 2 */
            idx_t m   = n / 3;
            seq_t a_m   = rec(m);
            seq_t a_m1 = rec(m + 1);
            if (a_m1 == 0) val = a_m + 1;
            else           val = a_m + a_m1;
        }
    }
    seen[n] = 1;
    cache[n] = val;
    return val;
}

/* External interface expected by the build system. */
seq_t a(idx_t n)
{
    return rec(n);
}
