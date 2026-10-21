/*
 * A251598  Dimension of space of invariant tensors in 2n-th tensor power of
 *          natural representation of Sp(8).
 * https://oeis.org/A251598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*(2*n-1)*(10*n^3+137*n^2+521*n+432)*a(n-1)
 *            -256*(2*n-1)*(n-1)*(n+3)*(2*n-3)*a(n-2)) /
 *            ((n+4)*(n+7)*(n+9)*(n+10)) a(0)=1, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* ---------- cache -------------------------------------------- */
    static seq_t *cache = NULL;            /* array of terms */
    static size_t  cap   = 0;              /* storage size */

    /* ensure we have space for indices 0..n */
    size_t need = (size_t)n + 1;
    if (need < 2) need = 2;
    if (need > cap) {
        size_t newcap = cap ? cap : 1;
        while (newcap < need) newcap <<= 1;
        /* allocate zero‑initialised memory for new cache */
        seq_t *tmp = (seq_t *)calloc(newcap, sizeof(seq_t));
        if (!tmp) abort();                /* fatal allocation failure   */
        if (cache) {
            for (size_t i = 0; i < cap; ++i) tmp[i] = cache[i];
            free(cache);
        }
        cache = tmp;
        cap   = newcap;
    }

    /* ---------- initial values ----------------------------------- */
    if (cache[0] == (seq_t)0) cache[0] = (seq_t)1;
    if (cache[1] == (seq_t)0) cache[1] = (seq_t)1;
    if (n == 0) return cache[0];
    if (n == 1) return cache[1];

    /* ---------- compute missing terms up to n ------------------- */
    for (idx_t i = 2; i <= n; ++i) {
        if (cache[(size_t)i] != (seq_t)0) continue;   /* already known   */

        /* use 128‑bit intermediate when seq_t is 64‑bit */
#if defined(__SIZEOF_INT128__) || defined(_MSC_VER)
        if (sizeof(seq_t) <= 8) {
            typedef unsigned __int128 u128;
            u128 two = (u128)i * 2u;
            u128 X   = (u128)4u * (two - 1u);
            u128 poly =  (u128)10u * i * i * i
                       + (u128)137u * i * i
                       + (u128)521u * i
                       + 432u;
            X = X * poly;

            u128 Y = (u128)256u * (two - 1u)
                     * (i - 1u)
                     * (i + 3u)
                     * (two - 3u);

            u128 num1 = X * (u128)cache[(size_t)(i-1)];
            u128 num2 = Y * (u128)cache[(size_t)(i-2)];
            u128 num  = num1 - num2;             /* num1 >= num2 for all n>=2 */

            u128 den = (u128)(i + 4u)
                       * (i + 7u)
                       * (i + 9u)
                       * (i + 10u);

            cache[(size_t)i] = (seq_t)(num / den);
            continue;
        }
#endif
        /* bigint or 128‑bit seq_t path --------------------------------- */
        seq_t two   = (seq_t)i * ((seq_t)2);
        seq_t X     = ((seq_t)4 * (two - (seq_t)1));
        seq_t poly  = ((seq_t)10 * i * i * i);
        poly = poly + ((seq_t)137 * i * i);
        poly = poly + ((seq_t)521 * i);
        poly = poly + (seq_t)432;
        X = X * poly;

        seq_t Y = ((seq_t)256 * (two - (seq_t)1));
        Y = Y * ((seq_t)(i - 1));
        Y = Y * ((seq_t)(i + 3));
        Y = Y * (two - (seq_t)3);

        seq_t num1 = X * cache[(size_t)(i-1)];
        seq_t num2 = Y * cache[(size_t)(i-2)];
        seq_t num  = num1 - num2;                 /* works for unsigned big int */

        seq_t den = ((seq_t)(i + 4) * (i + 7) * (i + 9) * (i + 10));
        cache[(size_t)i] = num / den;
    }
    return cache[(size_t)n];
}
