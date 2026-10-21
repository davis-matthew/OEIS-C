/*
 * A277345  a(n) = Gamma(n+1, phi)*exp(phi) + Gamma(n+1, 1-phi)*exp(1-phi),
 *          where phi=(1+sqrt(5))/2.
 * https://oeis.org/A277345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = L(n) + n*a(n-1), with a(0) = 2, and L(0)=2, L(1)=1,
 *            L(k)=L(k-1)+L(k-2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *lucas_cache = NULL;
    static size_t lucas_cap = 0;
    static seq_t *a_cache    = NULL;
    static size_t a_cap      = 0;

    /* --- compute Lucas numbers up to n --- */
    if (lucas_cap == 0) {
        lucas_cap  = 2;
        lucas_cache = (seq_t*)malloc(lucas_cap * sizeof(seq_t));
        if (!lucas_cache) return 0;
        lucas_cache[0] = (seq_t)2;
        lucas_cache[1] = (seq_t)1;
    }
    if (lucas_cap <= (size_t)n) {
        size_t old_cap = lucas_cap;
        lucas_cap = (size_t)n + 1;
        lucas_cache = (seq_t*)realloc(lucas_cache, lucas_cap * sizeof(seq_t));
        if (!lucas_cache) return 0;
        for (size_t i = old_cap; i <= (size_t)n; ++i) {
            lucas_cache[i] = lucas_cache[i-1] + lucas_cache[i-2];
        }
    }

    /* --- compute a(n) up to n --- */
    if (a_cap == 0) {
        a_cap = 1;
        a_cache = (seq_t*)malloc(a_cap * sizeof(seq_t));
        if (!a_cache) return 0;
        a_cache[0] = (seq_t)2;
    }
    if (a_cap <= (size_t)n) {
        size_t old_cap = a_cap;
        a_cap = (size_t)n + 1;
        a_cache = (seq_t*)realloc(a_cache, a_cap * sizeof(seq_t));
        if (!a_cache) return 0;
        for (size_t i = old_cap; i <= (size_t)n; ++i) {
            a_cache[i] = lucas_cache[i] + (seq_t)i * a_cache[i-1];
        }
    }

    return a_cache[n];
}
