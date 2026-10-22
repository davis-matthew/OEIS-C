/*
 * A336168  Garel's separator length for position n of the Thue-Morse sequence
 *          (A010060).
 * https://oeis.org/A336168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Garel's separator length for position n of the Thue-Morse sequence
   (OEIS A336168) computed via the recurrence relations given in the
   OEIS entry.
   Initial values:
       a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=3, a(5)=2,
       a(6)=5, a(7)=4.
   Recurrences (for n≥8):
     a(8*k)   = 3*a(4*k)   - 2*a(2*k)
     a(8*k+1) = 2*a(4*k)   + a(4*k+1) - 2*a(2*k)
     a(8*k+2) = 2*a(k)     - 4*a(2*k) + a(2*k+1)
               + a(4*k) + 2*a(4*k+1)
     a(8*k+3) = a(4*m+3)   with m=(n-3)/4:
               = 2*a(2*m)  - 2*a(2*m+1)
               - 2*a(4*m)  + a(4*m+1) + 2*a(4*m+2)
     a(8*k+4) = a(4*k) + 2*a(4*k+2) - 2*a(2*k)
     a(8*k+5) = 2*a(4*k+2) + 2*a(4*k) - 4*a(2*k)
     a(8*k+6) = 2*a(2*k) - 4*a(2*k+1) - 3*a(4*k)
               + 2*a(4*k+1) + 4*a(4*k+2)
   For indices ≡3 or 7 (mod 8) the recurrence for a(4*m+3) (with
   m=(n‑3)/4) yields the value. */

#include <stdlib.h>
#include <stdint.h>

/* Forward declaration from prelude: seq_t a(idx_t n); */

static seq_t *cache = NULL;
static unsigned char *computed = NULL;
static idx_t cache_cap = 0;

/* Ensure the cache can hold indices 0…n. */
static void ensure_cache(idx_t n) {
    if (n < cache_cap)
        return;
    idx_t new_cap = n + 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    computed = (unsigned char *)realloc(computed, new_cap * sizeof(unsigned char));
    for (idx_t i = cache_cap; i < new_cap; ++i)
        computed[i] = 0;
    cache_cap = new_cap;
}

/* Init first eight values (constant). */
static void init_constants(void) {
    if (cache_cap > 0 && computed[0])
        return;            /* already initialized */
    ensure_cache(7);
    cache[0] = 1; computed[0] = 1;
    cache[1] = 1; computed[1] = 1;
    cache[2] = 2; computed[2] = 1;
    cache[3] = 3; computed[3] = 1;
    cache[4] = 3; computed[4] = 1;
    cache[5] = 2; computed[5] = 1;
    cache[6] = 5; computed[6] = 1;
    cache[7] = 4; computed[7] = 1;
}

/* Recursive computation with memoisation. */
static void compute(idx_t n) {
    if (n < cache_cap && computed[n])
        return;
    ensure_cache(n);

    if (n < 8)            /* base case */
        return;

    int64_t t = 0;
    switch (n & 7) {      /* n % 8 */
        case 0: {          /* n = 8*k */
            idx_t k = n >> 3;
            int64_t a2 = (int64_t)a(2 * k);
            int64_t a4 = (int64_t)a(4 * k);
            t = 3 * a4 - 2 * a2;
            break;
        }
        case 1: {          /* n = 8*k+1 */
            idx_t k = n >> 3;
            int64_t a2  = (int64_t)a(2 * k);
            int64_t a4  = (int64_t)a(4 * k);
            int64_t a41 = (int64_t)a(4 * k + 1);
            t = 2 * a4 + a41 - 2 * a2;
            break;
        }
        case 2: {          /* n = 8*k+2 */
            idx_t k = n >> 3;
            int64_t a0  = (int64_t)a(k);
            int64_t a2  = (int64_t)a(2 * k);
            int64_t a21 = (int64_t)a(2 * k + 1);
            int64_t a4  = (int64_t)a(4 * k);
            int64_t a41 = (int64_t)a(4 * k + 1);
            t = 2 * a0 - 4 * a2 + a21 + a4 + 2 * a41;
            break;
        }
        case 3:           /* n = 4*m+3 (covers 8*k+3) */
        case 7: {          /* n = 8*k+7 – same recurrence */
            idx_t m = (n - 3) >> 2;
            int64_t a2  = (int64_t)a(2 * m);
            int64_t a21 = (int64_t)a(2 * m + 1);
            int64_t a4  = (int64_t)a(4 * m);
            int64_t a41 = (int64_t)a(4 * m + 1);
            int64_t a42 = (int64_t)a(4 * m + 2);
            t = 2 * a2 - 2 * a21 - 2 * a4 + a41 + 2 * a42;
            break;
        }
        case 4: {          /* n = 8*k+4 */
            idx_t k = n >> 3;
            int64_t a2  = (int64_t)a(2 * k);
            int64_t a4  = (int64_t)a(4 * k);
            int64_t a42 = (int64_t)a(4 * k + 2);
            t = a4 + 2 * a42 - 2 * a2;
            break;
        }
        case 5: {          /* n = 8*k+5 */
            idx_t k = n >> 3;
            int64_t a2  = (int64_t)a(2 * k);
            int64_t a4  = (int64_t)a(4 * k);
            int64_t a42 = (int64_t)a(4 * k + 2);
            t = 2 * a42 + 2 * a4 - 4 * a2;
            break;
        }
        case 6: {          /* n = 8*k+6 */
            idx_t k = n >> 3;
            int64_t a2  = (int64_t)a(2 * k);
            int64_t a21 = (int64_t)a(2 * k + 1);
            int64_t a4  = (int64_t)a(4 * k);
            int64_t a41 = (int64_t)a(4 * k + 1);
            int64_t a42 = (int64_t)a(4 * k + 2);
            t = 2 * a2 - 4 * a21 - 3 * a4 + 2 * a41 + 4 * a42;
            break;
        }
    }
    cache[n] = (seq_t)t;
    computed[n] = 1;
}

/* Public callable function with external linkage. */
seq_t a(idx_t n) {
    init_constants();
    if (n < cache_cap && computed[n])
        return cache[n];
    compute(n);
    return cache[n];
}
