/*
 * A260202  Number of (n+2) X (3+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000000 00000001 or 00010001.
 * https://oeis.org/A260202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 9*a(n-3) + 6*a(n-4) - 3*a(n-5) -
 *            10*a(n-6) - 17*a(n-7) - 7*a(n-8) + 12*a(n-9) + 12*a(n-10) -
 *            6*a(n-11) for n > 12 Initial values: a(1)=106, a(2)=207,
 *            a(3)=685, a(4)=1857, a(5)=5291, a(6)=15715, a(7)=45181,
 *            a(8)=129853, a(9)=377479, a(10)=1091843, a(11)=3153331,
 *            a(12)=91...
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 38, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* pre‑defined initial terms (index 0 unused) */
    static const seq_t init[13] = {
        0ULL,
        106ULL, 207ULL, 685ULL, 1857ULL, 5291ULL, 15715ULL,
        45181ULL, 129853ULL, 377479ULL, 1091843ULL,
        3153331ULL, 9128061ULL
    };

    if (n == 0) return 0;           /* undefined for n<1 – return 0 */
    if (n <= 12) return init[n];    /* return known value */

    /* ring buffer holding the last 11 terms:
       arr[0] = a[i-1], ..., arr[10] = a[i-11]   */
    seq_t arr[11];
    for (idx_t j = 0; j < 11; ++j)
        arr[j] = init[12 - j];       /* initialise with a[12]..a[2] */

    seq_t term = 0;
    for (idx_t i = 13; i <= n; ++i) {
        term = arr[0];
        term += arr[1] * (seq_t)2;
        term += arr[2] * (seq_t)9;
        term += arr[3] * (seq_t)6;
        term -= arr[4] * (seq_t)3;
        term -= arr[5] * (seq_t)10;
        term -= arr[6] * (seq_t)17;
        term -= arr[7] * (seq_t)7;
        term += arr[8] * (seq_t)12;
        term += arr[9] * (seq_t)12;
        term -= arr[10] * (seq_t)6;

        /* rotate buffer: discard arr[10], shift left, insert new term */
        for (idx_t j = 10; j > 0; --j)
            arr[j] = arr[j - 1];
        arr[0] = term;
    }

    return term;   /* term now holds a(n) */
}
