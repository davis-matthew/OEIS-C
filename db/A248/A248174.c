/*
 * A248174  2-adic order of the tribonacci sequence.
 * https://oeis.org/A248174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = v2(T_{n+2}) where * T(1)=0, T(2)=0, T(3)=1 and
 *            T(k)=T(k-1)+T(k-2)+T(k-3). * v2(x) is the exponent of the
 *            highest power of two dividing x
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for tribonacci numbers up to the largest n+2 seen so far */
    static idx_t max_comp = 0;
    static seq_t *trib = NULL;

    /* we need T(n+2) */
    idx_t need = n + 2;

    /* allocate or grow the cache */
    if (trib == NULL) {
        /* start with indices 1..3 initialized */
        size_t sz = (size_t)(need + 1);
        trib = (seq_t*)malloc(sz * sizeof(seq_t));
        if (!trib) return 0;                 /* allocation failed – return 0 */
        trib[1] = (seq_t)0;
        trib[2] = (seq_t)0;
        trib[3] = (seq_t)1;
        max_comp = 3;
    } else if (need > max_comp) {
        size_t sz = (size_t)(need + 1);
        trib = (seq_t*)realloc(trib, sz * sizeof(seq_t));
        if (!trib) return 0;                 /* allocation failed – return 0 */
        /* compute new tribonacci terms */
        for (idx_t i = max_comp + 1; i <= need; ++i) {
            trib[i] = trib[i-1] + trib[i-2] + trib[i-3];
        }
        max_comp = need;
    }

    /* get the tribonacci value modulo 2^bits(seq_t) */
    seq_t val = trib[need];

    /* count trailing zeros (2-adic order) */
    seq_t cnt = 0;
    while ((val & (seq_t)1) == (seq_t)0 && val != (seq_t)0) {
        val >>= 1;
        ++cnt;
    }

    /* val might be 0 if the true number is a multiple of 2^bits(seq_t);
     * in that case the exact order cannot be determined with the given type.
     * Return the maximum possible value (all bits are zero). */
    if (val == (seq_t)0) {
        return (seq_t)(sizeof(seq_t) * 8);
    }

    return cnt;
}
