/*
 * A260974  Number of (n+2)X(2+2) 0..1 arrays with each 3X3 subblock having
 *          clockwise perimeter pattern 00000001 00001001 or 00100101.
 * https://oeis.org/A260974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-2) + 2*a(n-3) + a(n-4) + 2*a(n-5) + 6*a(n-6) +
 *            5*a(n-7) * - 3*a(n-8) - 9*a(n-9) - 8*a(n-10) - 5*a(n-11) for
 *            n>14 * Initial values (n=1..14): * a(1)=91, a(2)=133, a(3)=239,
 *            a(4)=481, a(5)=925, a(6)=1873, * a(7)=3683, a(8)=7321,
 *            a(9)=14439, a(10)=28529, a(11)=56363, * a(12)=111523, a(1...
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 60, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;          /* cache[1..maxn]  */
    static idx_t maxn = 0;               /* highest computed n */

    /* hard‑coded first 14 terms */
    static const seq_t init[14] = {
        91, 133, 239, 481, 925, 1873,
        3683, 7321, 14439, 28529, 56363,
        111523, 220633, 436539
    };

    if (n == 0) return 0;               /* undefined for n<1 */

    if (n <= 14) return init[n-1];

    /* allocate or grow cache as needed */
    if (cache == NULL) {
        cache = (seq_t*)malloc(15 * sizeof(seq_t));
        for (idx_t i = 1; i <= 14; ++i) cache[i] = init[i-1];
        maxn = 14;
    }
    if (n > maxn) {
        size_t newsz = (size_t)(n + 1) * sizeof(seq_t);
        seq_t *tmp = (seq_t*)realloc(cache, newsz);
        if (!tmp) return 0; /* allocation failed */
        cache = tmp;
        /* compute missing terms */
        for (idx_t i = maxn + 1; i <= n; ++i) {
            seq_t t = cache[i-2]*2 + cache[i-3]*2 + cache[i-4]
                    + cache[i-5]*2 + cache[i-6]*6 + cache[i-7]*5;
            seq_t sub = cache[i-8]*3 + cache[i-9]*9
                      + cache[i-10]*8 + cache[i-11]*5;
            cache[i] = t - sub;   // result is guaranteed non‑negative
        }
        maxn = n;
    }
    return cache[n];
}
