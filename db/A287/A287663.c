/*
 * A287663  {0->1, 1->000}-transform of the infinite Fibonacci word A003849.
 * https://oeis.org/A287663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if the nth output symbol of the {0->1,1->000} transform
 *            of the infinite Fibonacci word is 1; otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t get_value(idx_t k, idx_t n, idx_t *outlen) {
    if (k == 0) {                /* w0 = "0" → output [1] */
        return (seq_t)1;
    }
    if (k == 1) {                /* w1 = "01" → output [1,0,0,0] */
        return (n == 1) ? (seq_t)1 : (seq_t)0;
    }
    if (n <= outlen[k - 1]) {
        return get_value(k - 1, n, outlen);
    } else {
        return get_value(k - 2, n - outlen[k - 1], outlen);
    }
}

seq_t a(idx_t n) {
    if (n == 0)                  /* undefined for offset 1; safe return */
        return (seq_t)0;

    /* build output-length array outlen[k] until outlen[k] ≥ n */
    idx_t *outlen = NULL;
    size_t cap = 4;
    outlen = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!outlen)
        return (seq_t)0;          /* allocation failure */

    outlen[0] = (idx_t)1;         /* w0 output length */
    outlen[1] = (idx_t)4;         /* w1 output length */
    size_t sz = 2;

    while (outlen[sz - 1] < n) {
        if (sz >= cap) {
            cap *= 2;
            idx_t *tmp = (idx_t *)realloc(outlen, cap * sizeof(idx_t));
            if (!tmp) {
                free(outlen);
                return (seq_t)0;
            }
            outlen = tmp;
        }
        idx_t prev1 = outlen[sz - 1];
        idx_t prev2 = outlen[sz - 2];
        idx_t next = prev1 + prev2;
        /* protect against overflow; if overflow occurs the loop would
           never reach a larger n, so break */
        if (next < prev1)
            break;
        outlen[sz] = next;
        ++sz;
    }

    idx_t k = sz - 1;              /* minimal k with outlen[k] ≥ n */
    seq_t result = get_value(k, n, outlen);

    free(outlen);
    return result;
}
