/*
 * A268443  a(n) = (A005705(n) - A268444(n))/4.
 * https://oeis.org/A268443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/4)*(b(n)-c(n)) where * b(0)=1, b(n)=b(n-1)+b(n/4) *
 *            c(n)=∏_(digits d of n in base 4)(d+1) (base case: c(0)=1) * This
 *            implements OEIS sequence A268443
 * Range:     u8 n <= 35, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>


seq_t a(idx_t n) {
    /* Memoized array for b(·) */
    static seq_t *b = NULL;
    static idx_t len = 0;          /* current size of b (>=1) */

    if (len == 0) {                /* initialise */
        len = 1;
        b = (seq_t *)malloc(sizeof(seq_t) * len);
        if (!b) return 0;          /* fallback, should not happen */
        b[0] = (seq_t)1;
    }

    if (n >= len) {                /* grow array as needed */
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(b, sizeof(seq_t) * new_len);
        if (!tmp) return 0;        /* fallback */
        b = tmp;
        for (idx_t i = len; i <= n; ++i) {
            b[i] = b[i-1] + b[i/4];
        }
        len = new_len;
    }

    /* compute c(n): product of (digit+1) over base‑4 digits */
    seq_t prod = (seq_t)1;
    idx_t t = n;
    if (t == 0) {
        prod = (seq_t)1;
    } else {
        while (t > 0) {
            uint8_t d = (uint8_t)(t & 3);   /* lowest base‑4 digit */
            prod *= ((seq_t)d + 1);
            t >>= 2;
        }
    }

    seq_t diff = b[n] - prod;   /* b(n)-c(n) is always divisible by 4 */
    return diff >> 2;           /* divide by 4 */
}
