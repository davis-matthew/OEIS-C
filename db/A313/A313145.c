/*
 * A313145  Coordination sequence Gal.6.264.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A313145: Coordination sequence Gal.6.264.1
   Recurrence (for n > 12):
      a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5)
             - 2*a(n-6) + 2*a(n-7) - 2*a(n-8) + 2*a(n-9)
             - 2*a(n-10) + 2*a(n-11) - a(n-12)
   Initial values:
      a(0)=1, a(1)=4, a(2)=9, a(3)=14, a(4)=20,
      a(5)=25, a(6)=29, a(7)=33, a(8)=38,
      a(9)=44, a(10)=49, a(11)=54, a(12)=58
*/
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;        /* dynamic cache of computed terms */
static idx_t cache_size = 0;       /* next free index (size of cache) */

/* Return the n‑th term of the sequence.  Cache values so repeated
   calls are O(1).  The recurrence is evaluated using only addition
   and subtraction so it works both for fixed‑width unsigned types
   and for the arbitrary‑precision runtime type. */
seq_t a(idx_t n)
{
    /* Initialise the cache with the 13 known initial terms on first use. */
    if (!cache) {
        cache = (seq_t *)malloc(13 * sizeof(seq_t));
        if (!cache) exit(1);
        cache_size = 13;
        cache[0]  = 1;
        cache[1]  = 4;
        cache[2]  = 9;
        cache[3]  = 14;
        cache[4]  = 20;
        cache[5]  = 25;
        cache[6]  = 29;
        cache[7]  = 33;
        cache[8]  = 38;
        cache[9]  = 44;
        cache[10] = 49;
        cache[11] = 54;
        cache[12] = 58;
    }

    /* If the value is already cached, return it immediately. */
    if (n < cache_size)
        return cache[n];

    /* Expand the cache to hold values up to index n.  realloc may
       move the buffer, so we cast the return value to the correct type. */
    cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!cache) exit(1);

    /* Compute the missing terms using the recurrence. */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* Compute a(i) = 2*a(i-1) - 2*a(i-2) + ... - a(i-12) using
           only addition and subtraction. */
        seq_t cur = 0;
        /* 2*a(i-1) */
        cur += cache[i-1];
        cur += cache[i-1];
        /* -2*a(i-2) */
        cur -= cache[i-2];
        cur -= cache[i-2];
        /* +2*a(i-3) */
        cur += cache[i-3];
        cur += cache[i-3];
        /* -2*a(i-4) */
        cur -= cache[i-4];
        cur -= cache[i-4];
        /* +2*a(i-5) */
        cur += cache[i-5];
        cur += cache[i-5];
        /* -2*a(i-6) */
        cur -= cache[i-6];
        cur -= cache[i-6];
        /* +2*a(i-7) */
        cur += cache[i-7];
        cur += cache[i-7];
        /* -2*a(i-8) */
        cur -= cache[i-8];
        cur -= cache[i-8];
        /* +2*a(i-9) */
        cur += cache[i-9];
        cur += cache[i-9];
        /* -2*a(i-10) */
        cur -= cache[i-10];
        cur -= cache[i-10];
        /* +2*a(i-11) */
        cur += cache[i-11];
        cur += cache[i-11];
        /* -a(i-12) */
        cur -= cache[i-12];

        cache[i] = cur;
    }

    cache_size = n + 1;
    return cache[n];
}
