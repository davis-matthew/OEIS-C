/*
 * A302317  Number of nX3 0..1 arrays with every element equal to 0, 1, 2 or 4
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            3*a(n-1)+3*a(n-2)+30*a(n-3)-88*a(n-4)-36*a(n-5)-60*a(n-6)-116*a(n-7)+4*a(n-8)-16*a(n-9)+20*a(n-10)+40*a(n-11)+64*a(n-12)+16*a(n-13)-32*a(n-14)
 *            Initial values: a(1)=4, a(2)=20, a(3)=65, a(4)=281, a(5)=1144,
 *            a(6)=4321, a(7)=18093, a(8)=72746, a(9)=285199, a(10)=1170837,
 *            a(11)=4701572, a(12)=...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;

    /* static cache */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t length = 0;  /* number of terms stored */

    /* expand cache if needed */
    if ((size_t)n > capacity) {
        size_t new_cap = capacity > 0 ? capacity : 1;
        while (new_cap < (size_t)n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    /* compute terms up to n */
    while (length < (size_t)n) {
        size_t i = length;          /* index in cache (0‑based) -> a(i+1) */
        seq_t val;
        switch (i) {                /* initial 14 terms */
            case 0: val = 4;                       break;
            case 1: val = 20;                      break;
            case 2: val = 65;                      break;
            case 3: val = 281;                     break;
            case 4: val = 1144;                    break;
            case 5: val = 4321;                    break;
            case 6: val = 18093;                   break;
            case 7: val = 72746;                   break;
            case 8: val = 285199;                  break;
            case 9: val = 1170837;                 break;
            case 10: val = 4701572;                break;
            case 11: val = 18728977;               break;
            case 12: val = 76116025;               break;
            case 13: val = 305826310;              break;
            default:
                /* recurrence for n ≥ 15 (i ≥ 14) */
                val  = 3*cache[i-1] + 3*cache[i-2] + 30*cache[i-3]
                     - 88*cache[i-4] - 36*cache[i-5] - 60*cache[i-6]
                     - 116*cache[i-7] + 4*cache[i-8]
                     - 16*cache[i-9] + 20*cache[i-10]
                     + 40*cache[i-11] + 64*cache[i-12]
                     + 16*cache[i-13] - 32*cache[i-14];
        }
        cache[i] = val;
        ++length;
    }

    return cache[n-1];
}
