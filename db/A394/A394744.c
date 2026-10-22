/*
 * A394744  a(n) = number of triples (x, y, z) such that x^2 + y*z = n, where
 *          x,y,z are distinct nonzero Fibonacci numbers and y < z.
 * https://oeis.org/A394744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (x, y, z) of distinct, non‑zero
 *            Fibonacci numbers such that x^2 + y*z = n and y < z
 * Range:     u8 n <= 20, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Generate the list of distinct non‑zero Fibonacci numbers <= n. */
    /*  1, 2, 3, 5, 8, 13, ...  */
    seq_t fib[64];
    size_t m = 0;                 /* number of Fibonacci numbers collected */

    /* Initial unique Fibonacci number 1 */
    fib[m++] = (seq_t)1;

    seq_t f1 = (seq_t)1, f2 = (seq_t)1;  /* Fibonacci seeds f1 = F(1), f2 = F(2) */

    while (1) {
        seq_t f3 = f1 + f2;
        if (f3 > (seq_t)n)        /* no need to include larger Fibonacci numbers */
            break;
        if (f3 != fib[m-1])       /* avoid duplicates (the value 1 appears twice) */
            fib[m++] = f3;
        f1 = f2;
        f2 = f3;
    }

    seq_t count = 0;

    /* Test all triples (x, y, z) with distinct Fibonacci values. */
    for (size_t ix = 0; ix < m; ++ix) {
        seq_t x = fib[ix];
        seq_t x2 = x * x;
        if (x2 > (seq_t)n)          /* x^2 already exceeds n */
            continue;

        for (size_t iy = 0; iy < m; ++iy) {
            if (iy == ix)           /* y must differ from x */
                continue;
            seq_t y = fib[iy];

            for (size_t iz = iy + 1; iz < m; ++iz) {
                if (iz == ix)       /* z must differ from x (and from y by index) */
                    continue;
                seq_t z = fib[iz];

                seq_t val = x2 + y * z;
                if (val == (seq_t)n)
                    ++count;
            }
        }
    }

    return count;
}
