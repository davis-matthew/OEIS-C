/*
 * A316450  Number of nX3 0..1 arrays with every element unequal to 1, 2, 4,
 *          5, 6 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 25, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence (valid for n > 20):
 *   a(n) =  a(n-1) +3*a(n-2) +10*a(n-3)
 *          -4*a(n-4) -29*a(n-5) -55*a(n-6)
 *          -11*a(n-7) +84*a(n-8) +149*a(n-9)
 *          +92*a(n-10) -68*a(n-11) -115*a(n-12)
 *          -73*a(n-13) -44*a(n-14) -67*a(n-15)
 *          -36*a(n-16) -4*a(n-17)
 *
 * Initial values (offset 1):
 *   a(1)=1, a(2)=11, a(3)=9, a(4)=23, a(5)=58, a(6)=126,
 *   a(7)=272, a(8)=672, a(9)=1553, a(10)=3555, a(11)=8624,
 *   a(12)=20434, a(13)=48097, a(14)=116009, a(15)=277803,
 *   a(16)=663233, a(17)=1596067, a(18)=3838951, a(19)=9220294,
 *   a(20)=22194498
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* Precomputed values 1..20 */
static const seq_t init_vals[21] = {
    0ULL,          /* dummy for index 0 */
    1ULL, 11ULL, 9ULL, 23ULL, 58ULL, 126ULL, 272ULL, 672ULL,
    1553ULL, 3555ULL, 8624ULL, 20434ULL, 48097ULL, 116009ULL,
    277803ULL, 663233ULL, 1596067ULL, 3838951ULL, 9220294ULL,
    22194498ULL
};

seq_t a(idx_t n) {
    if (n < 1) return 0;               /* offset is 1 */

    static seq_t *buf = NULL;          /* 1-based array */
    static size_t cap = 0;
    static idx_t max_computed = 0;

    /* If the buffer is too small, grow it */
    if ((size_t)n > cap) {
        size_t old_cap = cap;
        size_t new_cap = cap ? cap : 1;
        while (new_cap < (size_t)n) new_cap <<= 1;

        /* Reallocate memory */
        void *p = realloc(buf, (new_cap + 1) * sizeof(seq_t));
        if (!p) {
            /* In a real program we might handle this better,
               but abort here keeps the code simple */
            exit(1);
        }
        buf = (seq_t *)p;

        /* If seq_t is a class type, construct the new elements
           (this part is compiled only in C++). */
#ifdef __cplusplus
        for (size_t i = old_cap + 1; i <= new_cap; ++i) {
            new (buf + i) seq_t();
        }
#endif
        /* Ensure element 0 exists and is zero */
#ifdef __cplusplus
        new (buf + 0) seq_t();
#endif
        buf[0] = (seq_t)0;
        cap = new_cap;
    }

    /* If the value has already been computed, return it */
    if (max_computed >= n) return buf[n];

    /* Compute missing values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 20) {
            buf[i] = init_vals[i];
        } else {
            /* Compute using the empirical recurrence */
            seq_t val = buf[i-1];
            val += 3 * buf[i-2];
            val += 10 * buf[i-3];
            val -= 4 * buf[i-4];
            val -= 29 * buf[i-5];
            val -= 55 * buf[i-6];
            val -= 11 * buf[i-7];
            val += 84 * buf[i-8];
            val += 149 * buf[i-9];
            val += 92 * buf[i-10];
            val -= 68 * buf[i-11];
            val -= 115 * buf[i-12];
            val -= 73 * buf[i-13];
            val -= 44 * buf[i-14];
            val -= 67 * buf[i-15];
            val -= 36 * buf[i-16];
            val -= 4 * buf[i-17];
            buf[i] = val;
        }
    }

    max_computed = n;
    return buf[n];
}
