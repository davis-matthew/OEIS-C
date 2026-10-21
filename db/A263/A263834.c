/*
 * A263834  Sprague-Grundy sequence G(i-Mark({2,4}, {2})).
 * https://oeis.org/A263834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   For n < 11 : use the precomputed values a(0..10). For n ≥ 11:
 *            Let r = n mod 6. If r==1 or r==4 → 0 If r==2 or r==3 → 1 If r==5
 *            → 2 If r==0 → let k = n/6. * If k is a power of 2: exponent e =
 *            v₂(k) → 2 if e is odd, 3 if e is even * If k is not a power of
 *            2: v = v₂(k) → 2 if v is even, 3 if v is od...
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* first 11 values are known */
    static const seq_t tbl[] = {
        0,0,1,1,2,2,0,0,1,1,3
    };
    if (n < sizeof(tbl)/sizeof(tbl[0]))
        return tbl[n];

    idx_t r = n % 6;
    switch (r) {
    case 1: case 4: return (seq_t)0;
    case 2: case 3: return (seq_t)1;
    case 5:        return (seq_t)2;
    case 0:        break; /* handle below */
    }

    idx_t k = n / 6;           /* n ≥ 12 here, so k ≥ 2 */

    /* test if k is a power of 2 */
    if ((k & (k - 1)) == 0) {
        /* compute exponent e of k */
        unsigned e = 0;
        idx_t t = k;
        while ((t & 1) == 0) {
            t >>= 1;
            e++;
        }
        return (e & 1) ? (seq_t)2 : (seq_t)3;
    } else {
        /* compute v₂(k) */
        unsigned v = 0;
        idx_t t = k;
        while ((t & 1) == 0) {
            t >>= 1;
            v++;
        }
        return (v & 1) ? (seq_t)3 : (seq_t)2;
    }

    /* unreachable */
    return (seq_t)0;
}
