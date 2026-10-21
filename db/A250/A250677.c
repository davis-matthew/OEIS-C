/*
 * A250677  Number of (1+1) X (n+1) 0..3 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 21*a(n-2) - 54*a(n-3) + 155*a(n-4) +
 *            118*a(n-5) - 228*a(n-6) - 144*a(n-7) Initial values: a(1)=104,
 *            a(2)=669, a(3)=3927, a(4)=22119, a(5)=120233, a(6)=637948,
 *            a(7)=3321772
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for storing previously computed values */
static seq_t *cache = NULL;
static idx_t max_n = 0;

/* Ensure the cache can hold terms up to n */
static void ensure_capacity(idx_t n) {
    if (n <= max_n) return;                /* already enough */
    idx_t new_size = n + 1;                /* indices 0..n, 0 unused */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    /* ignore allocation failure – cache remains unchanged */
    if (!new_cache) return;
    cache = new_cache;

    /* compute new terms */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        if (i == 1) cache[i] = (seq_t)104;
        else if (i == 2) cache[i] = (seq_t)669;
        else if (i == 3) cache[i] = (seq_t)3927;
        else if (i == 4) cache[i] = (seq_t)22119;
        else if (i == 5) cache[i] = (seq_t)120233;
        else if (i == 6) cache[i] = (seq_t)637948;
        else if (i == 7) cache[i] = (seq_t)3321772;
        else {
            seq_t t1 = (seq_t)10 * cache[i-1];
            seq_t t2 = (seq_t)21 * cache[i-2];
            seq_t t3 = (seq_t)54 * cache[i-3];
            seq_t t4 = (seq_t)155 * cache[i-4];
            seq_t t5 = (seq_t)118 * cache[i-5];
            seq_t t6 = (seq_t)228 * cache[i-6];
            seq_t t7 = (seq_t)144 * cache[i-7];
            cache[i] = t1 - t2 - t3 + t4 + t5 - t6 - t7;
        }
    }
    max_n = n;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;      /* outside offset, unused */
    ensure_capacity(n);
    return cache[n];
}
