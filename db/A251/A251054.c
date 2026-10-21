/*
 * A251054  Number of (n+1)X(7+1) 0..3 arrays with no 2X2 subblock having the
 *          sum of its diagonal elements greater than the absolute difference
 *          of its antidiagonal elements.
 * https://oeis.org/A251054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 16, u64 n <= 42, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A251054: Number of (n+1)x8 0..3 arrays with no 2×2 subblock
   having the sum of its diagonal elements greater than the absolute
   difference of its antidiagonal elements.
   Empirical linear recurrence:
     a(n) = 8*a(n-1) - 25*a(n-2) + 35*a(n-3) - 7*a(n-4)
            - 49*a(n-5) + 77*a(n-6) - 55*a(n-7) + 20*a(n-8) - 3*a(n-9)
        for n > 12
   Initial values (n ≥ 1):
     a(1)=29759, a(2)=118925, a(3)=237143, a(4)=533327,
     a(5)=1045337, a(6)=2129846, a(7)=4118693, a(8)=8207451,
     a(9)=16293926, a(10)=33460774, a(11)=69949629, a(12)=150556699 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined below the offset */

    /* Cache for previously computed values (1‑based indexing) */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t max = 0;

    /* Ensure cache can hold index n */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;          /* include index 0…n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                      /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Initial values, 1‑based */
    static const seq_t init[13] = {
        0ULL,          /* unused 0‑th index */
        29759ULL,      /* a(1) */
        118925ULL,     /* a(2) */
        237143ULL,     /* a(3) */
        533327ULL,     /* a(4) */
        1045337ULL,    /* a(5) */
        2129846ULL,    /* a(6) */
        4118693ULL,    /* a(7) */
        8207451ULL,    /* a(8) */
        16293926ULL,   /* a(9) */
        33460774ULL,   /* a(10) */
        69949629ULL,   /* a(11) */
        150556699ULL   /* a(12) */
    };

    if (n <= 12) return init[(size_t)n];

    /* Compute all missing terms up to n */
    for (size_t k = max + 1; k <= (size_t)n; ++k) {
        if (k <= 12) {                      /* still within the initial table */
            cache[k] = init[(size_t)k];
            continue;
        }
        /* Linear combination.  The type seq_t must support signed
           results for the temporary expressions; typical big‑int
           classes used in the OEIS builds are signed. */
        seq_t sum = 0;
        sum += 8 * cache[k-1];
        sum -= 25 * cache[k-2];
        sum += 35 * cache[k-3];
        sum -= 7 * cache[k-4];
        sum -= 49 * cache[k-5];
        sum += 77 * cache[k-6];
        sum -= 55 * cache[k-7];
        sum += 20 * cache[k-8];
        sum -= 3 * cache[k-9];
        cache[k] = sum;
    }

    max = (size_t)n;
    return cache[(size_t)n];
}
