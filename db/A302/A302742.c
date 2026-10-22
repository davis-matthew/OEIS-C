/*
 * A302742  Number of 3Xn 0..1 arrays with every element equal to 0, 1, 2, 3
 *          or 4 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A302742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=4, a(2)=32, a(3)=228, a(4)=1637, a(5)=11814, a(6)=85268,
 *            a(7)=615589, a(8)=4444226; For n>8:
 *            a(n)=7*a(n-1)+4*a(n-2)-17*a(n-3)-3*a(n-4)-9*a(n-6)+14*a(n-7)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The code uses a simple memoised recurrence.  It allocates an array
    dynamically, extends it as needed, and computes any a(n) in O(n).  */

#include <stdlib.h>

/* static storage for memoisation */
static seq_t *a_cache = NULL;
static size_t a_cache_len = 0;   /* largest n that has been computed */
static size_t a_cache_cap = 0;   /* allocated capacity (>= a_cache_len+1) */

/* ensure that capacity for at least N+1 elements is present */
static void a_ensure_cache(idx_t N)
{
    if (a_cache == NULL) {                     /* first allocation */
        a_cache_cap = (N + 1 > 16) ? N + 1 : 16;
        a_cache = (seq_t *)malloc(a_cache_cap * sizeof(seq_t));
        if (a_cache == NULL) abort();
        /* initialise base values up to 8 or N */
        a_cache[0] = 0;
        a_cache[1] = 4;
        a_cache[2] = 32;
        a_cache[3] = 228;
        a_cache[4] = 1637;
        a_cache[5] = 11814;
        a_cache[6] = 85268;
        a_cache[7] = 615589;
        a_cache[8] = 4444226;
        a_cache_len = (N < 8) ? (size_t)N : 8;
    }
    else if ((size_t)N + 1 > a_cache_cap) {    /* extend when needed */
        size_t new_cap = a_cache_cap * 2;
        if (new_cap < (size_t)N + 1) new_cap = (size_t)N + 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        a_cache = tmp;
        a_cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined, but safe */

    a_ensure_cache(n);

    /* compute values up to n if not already done */
    if ((size_t)n > a_cache_len) {
        idx_t i;
        for (i = (idx_t)(a_cache_len + 1); i <= n; ++i) {
            if (i <= 8) continue;            /* base values already set */
            seq_t pos = 7 * a_cache[i-1];
            pos += 4 * a_cache[i-2];
            pos += 14 * a_cache[i-7];

            seq_t neg = 17 * a_cache[i-3];
            neg += 3 * a_cache[i-4];
            neg += 9 * a_cache[i-6];

            a_cache[i] = pos - neg;          /* pos >= neg is guaranteed */
        }
        a_cache_len = (size_t)n;
    }

    return a_cache[n];
}
