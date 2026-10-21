/*
 * A267242  Number of nX5 binary arrays with row sums nondecreasing and
 *          columns lexicographically nondecreasing.
 * https://oeis.org/A267242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence * a(n) = 52*a(n-1)
 *            -1196*a(n-2) +16140*a(n-3) -142918*a(n-4) * +879116*a(n-5)
 *            -3875668*a(n-6) +12442580*a(n-7) * -29232481*a(n-8)
 *            +50015232*a(n-9) -61355336*a(n-10) * +52355680*a(n-11)
 *            -29405200*a(n-12) +9744000*a(n-13) * -1440000*a(n-14) * *
 *            Initial values (a...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Coefficients for the 14‑term recurrence */
static const long long coeffs[14] = {
    52LL,          /* a(n-1)   */
   -1196LL,       /* a(n-2)   */
    16140LL,      /* a(n-3)   */
   -142918LL,     /* a(n-4)   */
    879116LL,     /* a(n-5)   */
   -3875668LL,    /* a(n-6)   */
   12442580LL,    /* a(n-7)   */
   -29232481LL,   /* a(n-8)   */
   50015232LL,    /* a(n-9)   */
  -61355336LL,    /* a(n-10)  */
   52355680LL,    /* a(n-11)  */
  -29405200LL,    /* a(n-12)  */
   9744000LL,     /* a(n-13)  */
  -1440000LL      /* a(n-14)  */
};

/* Initial terms a(1) .. a(14) */
static const seq_t init_vals[14] = {
    (seq_t)6ULL,
    (seq_t)34ULL,
    (seq_t)232ULL,
    (seq_t)1986ULL,
    (seq_t)20040ULL,
    (seq_t)220235ULL,
    (seq_t)2499080ULL,
    (seq_t)28501471ULL,
    (seq_t)323067002ULL,
    (seq_t)3626695952ULL,
    (seq_t)40306404192ULL,
    (seq_t)443852375808ULL,
    (seq_t)4848323701804ULL,
    (seq_t)52590398731297ULL
};

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined offset, return 0 */

    if (n <= 14ULL) {
        /* direct lookup for the first 14 terms */
        return init_vals[n - 1ULL];
    }

    /* array holding the last 14 results: vals[0] = a(k-13), … , vals[13] = a(k)  */
    seq_t vals[14];
    for (size_t i = 0; i < 14; ++i) {
        vals[i] = init_vals[i];
    }

    seq_t next = 0;
    /* compute terms from a(15) up to a(n) */
    for (idx_t cur = 15ULL; cur <= n; ++cur) {
        next = 0;
        /* a(cur) = Σ coeffs[i] * a(cur-i-1)   (i = 0..13) */
        for (size_t i = 0; i < 14; ++i) {
            /* index of a(cur-i-1) in vals[] */
            size_t idx = 13 - i;   /* since vals[13] = a(cur-1) */
            seq_t term = vals[idx];
            long long c = coeffs[i];
            if (c >= 0) {
                next += term * (seq_t)c;
            } else {
                next -= term * (seq_t)(-c);
            }
        }
        /* shift the window: drop the oldest term, add the new one */
        for (size_t j = 0; j < 13; ++j) {
            vals[j] = vals[j + 1];
        }
        vals[13] = next;
    }

    return next;
}
