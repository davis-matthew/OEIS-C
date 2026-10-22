/*
 * A396409  Number of iterations of x -> x XOR 2^A000120(x) from x = n to
 *          reach a number of the form 2^m-1.
 * https://oeis.org/A396409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0) = 0, a(1) = 0; Recurrence: a(2*n+1) = a(n); a(2*n) = a(n) +
 *            popcount(n) + popcount(n+1) - 1 where popcount(x) is the number
 *            of 1‑bits of x
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* popcount for 64‑bit integers – builtin if available, else loop. */
static inline idx_t popcnt(idx_t x) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    idx_t c = 0;
    while (x) { c++; x &= x-1; }
    return c;
#endif
}

/* Recursive computation of a(n). */
static inline seq_t a_rec(idx_t n) {
    if (n <= 1)          return (seq_t)0;
    if (n & 1ULL)       /* odd:  a(2*k+1) = a(k) */
        return a_rec(n >> 1);
    else {              /* even: a(2*k)   = a(k) + popcnt(k) + popcnt(k+1) - 1 */
        idx_t k = n >> 1;
        seq_t res = a_rec(k);
        res += (seq_t)popcnt(k) + (seq_t)popcnt(k + 1) - 1;
        return res;
    }
}

seq_t a(idx_t n) {
    return a_rec(n);
}
