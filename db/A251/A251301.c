/*
 * A251301  Number of (n+1) X (1+1) 0..3 arrays with no 2 X 2 subblock having
 *          the sum of its diagonal elements less than the maximum of its
 *          antidiagonal elements.
 * https://oeis.org/A251301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A251301: Number of (n+1) X (1+1) 0..3 arrays with no 2 X 2 subblock
   having the sum of its diagonal elements less than the maximum of its
   antidiagonal elements.
   Recurrence (empirical):
     a(n) = 16*a(n‑1) – 60*a(n‑2) + 116*a(n‑3) – 104*a(n‑4)
            + 48*a(n‑5) – 11*a(n‑6) + a(n‑7)
   Initial values (n = 1 … 7):
     a(1)=196, a(2)=2292, a(3)=26664, a(4)=310224, a(5)=3609989,
     a(6)=42010273, a(7)=488885832
*/
#include <stdlib.h>

static seq_t *cache = NULL;      /* array holding computed values (index starts at 1) */
static size_t   cap   = 0;        /* capacity of the allocated array (excluding index 0) */
static idx_t    last  = 0;        /* highest index that has been computed */

/* Grow the cache so that indices 0..n are available.
   The array stores indices from 0 to cap inclusive, but index 0 is unused. */
static void ensure_cache(idx_t n)
{
    if (n <= last) return;              /* already computed up to n */

    /* Grow capacity if necessary */
    if (n + 1 > cap) {
        size_t newcap = (cap == 0) ? 8 : cap * 2;
        while (newcap < n + 1) newcap *= 2;

        seq_t *tmp = (seq_t*)realloc(cache, (newcap + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = newcap;
    }

    /* Compute new values from last+1 up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t a1 = cache[i-1];
        seq_t a2 = cache[i-2];
        seq_t a3 = cache[i-3];
        seq_t a4 = cache[i-4];
        seq_t a5 = cache[i-5];
        seq_t a6 = cache[i-6];
        seq_t a7 = cache[i-7];
        seq_t val = (seq_t)16 * a1;
        val -= (seq_t)60 * a2;
        val += (seq_t)116 * a3;
        val -= (seq_t)104 * a4;
        val += (seq_t)48 * a5;
        val -= (seq_t)11 * a6;
        val += a7;
        cache[i] = val;
    }
    last = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0, return 0 */

    /* Initialize cache with first seven values the first time a() is called */
    if (!cache) {
        cache = (seq_t*)malloc((7 + 1) * sizeof(seq_t));
        if (!cache) abort();
        cap   = 7;
        cache[1] = (seq_t)196;
        cache[2] = (seq_t)2292;
        cache[3] = (seq_t)26664;
        cache[4] = (seq_t)310224;
        cache[5] = (seq_t)3609989;
        cache[6] = (seq_t)42010273;
        cache[7] = (seq_t)488885832;
        last  = 7;
    }
    ensure_cache(n);
    return cache[n];
}
