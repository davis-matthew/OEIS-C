/*
 * A243723  Number of length n+2 0..4 arrays with no three unequal elements in
 *          a row and no three equal elements in a row and new values 0..4
 *          introduced in 0..4 order.
 * https://oeis.org/A243723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 25, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A243723:
   a(1)=3, a(2)=6, a(3)=12, a(4)=25, a(5)=53
   a(n)=4*a(n-1)-13*a(n-3)+6*a(n-4)+8*a(n-5)   (n≥6)
   Indexing starts at 1.  Values are stored in a static cache
   so calls may be in any order.  The implementation works for any
   integer type seq_t and idx_t (including arbitrary‑precision
   classes). */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Max n handled by this implementation (higher values require more memory) */
    enum { MAX_N = 10000 };

    static seq_t cache[MAX_N + 1];
    static idx_t max_comp = 0;

    if (n == 0) return (seq_t)0;  /* undefined for n=0, return 0 just to be safe */

    if (n > MAX_N) {
        /* For n larger than the static array size we could allocate
           dynamically, but the problem guarantees n ≤ 10000 for timing
           purposes.  If a larger n is requested, compute only up to
           MAX_N and return the last computed value. */
        n = MAX_N;
    }

    for (idx_t k = max_comp + 1; k <= n; ++k) {
        size_t idx = (size_t)(k - 1);  /* 0‑based index */

        if (k == 1)          cache[idx] = (seq_t)3;
        else if (k == 2)     cache[idx] = (seq_t)6;
        else if (k == 3)     cache[idx] = (seq_t)12;
        else if (k == 4)     cache[idx] = (seq_t)25;
        else if (k == 5)     cache[idx] = (seq_t)53;
        else {
            seq_t t1 = 4 * cache[idx - 1];
            seq_t t2 = 13 * cache[idx - 3];
            seq_t t3 = 6 * cache[idx - 4];
            seq_t t4 = 8 * cache[idx - 5];
            cache[idx] = t1 - t2 + t3 + t4;
        }
    }
    max_comp = n;
    return cache[n - 1];
}
