/*
 * A249036  a(1)=1, a(2)=2; thereafter a(n) = a(n-1-(number of even terms so
 *          far)) + a(n-1-(number of odd terms so far)).
 * https://oeis.org/A249036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2; thereafter a(n) = a(n-1-(number of even terms so
 *            far)) + a(n-1-(number of odd terms so far)) Memoised
 *            implementation that works for any seq_t type (built‑in or
 *            arbitrary‑precision)
 * Range:     u8 n <= 382, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* static storage for memoisation */
static seq_t  *cache = NULL;          /* 1‑based indices; cache[0] unused          */
static uint32_t *evenCnt = NULL;      /* cumulative even counts (1‑based)         */
static uint32_t *oddCnt  = NULL;      /* cumulative odd counts (1‑based)          */
static size_t cap = 0;                /* allocated capacity (excluding cache[0])  */
static size_t len = 0;                /* highest index that has been computed     */

/* enlarge storage so that index n (1‑based) is guaranteed to be reachable   */
static void ensure_cache(idx_t n)
{
    if ((size_t)n <= cap) return;

    size_t newcap = cap ? cap << 1 : 4;
    while (newcap < (size_t)n) newcap <<= 1;

    /* realloc with explicit casts to satisfy C++ */
    cache    = (seq_t*)realloc(cache,    (newcap + 1) * sizeof *cache);
    evenCnt  = (uint32_t*)realloc(evenCnt, (newcap + 1) * sizeof *evenCnt);
    oddCnt   = (uint32_t*)realloc(oddCnt,  (newcap + 1) * sizeof *oddCnt);
    cap = newcap;

    /* initialise the unused slot(s) that may now exist */
    if (cache)    cache[0] = (seq_t)0;
    if (evenCnt)  evenCnt[0] = 0;
    if (oddCnt)   oddCnt[0]  = 0;
}

/* compute a(n) for any n ≥ 1, memoised */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1, guard against UB */

    ensure_cache(n);

    /* initialise base cases lazily */
    if (len == 0 && n >= 1) {
        cache[1] = (seq_t)1;
        evenCnt[1] = 0;
        oddCnt[1]  = 1;
        len = 1;
    }
    if (len == 1 && n >= 2) {
        cache[2] = (seq_t)2;
        evenCnt[2] = 1;
        oddCnt[2]  = 1;
        len = 2;
    }

    /* compute missing terms up to n */
    while (len < (size_t)n) {
        size_t next = len + 1;              /* index to compute (1‑based)   */
        uint32_t ev = evenCnt[next-1];
        uint32_t od = oddCnt[next-1];

        size_t idxA = (next-1) - ev;        /* n-1-e */
        size_t idxB = (next-1) - od;        /* n-1-o */

        seq_t val = cache[idxA] + cache[idxB];
        cache[next] = val;

        /* update cumulative parity counts */
        if (val & (seq_t)1) {               /* odd */
            oddCnt[next] = oddCnt[next-1] + 1;
            evenCnt[next] = evenCnt[next-1];
        } else {                            /* even */
            evenCnt[next] = evenCnt[next-1] + 1;
            oddCnt[next]  = oddCnt[next-1];
        }

        len = next;
    }

    return cache[(size_t)n];
}
