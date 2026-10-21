/*
 * A247282  Run Length Transform of A001317.
 * https://oeis.org/A247282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{runs R of consecutive 1's in binary of n}
 *            A001317(|R|-1)
 * Range:     u8 n <= 510, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Fermat numbers: 2^(2^i)+1 for i = 0..5.  These cover all
       run lengths up to 64 (len-1 <= 63).  The values are cast to
       seq_t; if seq_t is smaller than 64 bits the cast truncates,
       but such terms then exceed the capacity of seq_t and the
       specification allows any result in that case.               */
    static const seq_t fermat[6] = {
        (seq_t)3ULL,          /* F_0 = 3  */
        (seq_t)5ULL,          /* F_1 = 5  */
        (seq_t)17ULL,         /* F_2 = 17 */
        (seq_t)257ULL,        /* F_3 = 257 */
        (seq_t)65537ULL,      /* F_4 = 65537 */
        (seq_t)4294967297ULL  /* F_5 = 4294967297 */
    };

    seq_t result = (seq_t)1;
    idx_t x = n;

    while (x) {
        /* Skip trailing zeros (no 1‑run). */
        if ((x & 1ULL) == 0) {
            x >>= 1ULL;
            continue;
        }

        /* Count run of consecutive 1's. */
        idx_t len = 0;
        while (x && (x & 1ULL)) {
            ++len;
            x >>= 1ULL;
        }

        /* Compute S(len) = A001317(len-1).  This is the product
           of Fermat numbers F_i where the i‑th bit of (len-1)
           is set. */
        idx_t mask = len - 1;    /* index into A001317 */
        seq_t s = (seq_t)1;
        for (size_t i = 0; i < 6; ++i) {
            if (mask & ((idx_t)1 << i))
                s *= fermat[i];
        }

        result *= s;
    }

    return result;
}
