/*
 * A390220  a(n) = Sum_{k=0..floor(n/2)} binomial(k,4*n-8*k).
 * https://oeis.org/A390220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(k, 4*n-8*k)
 * Range:     u8 n <= 22, u16 n <= 39, u32 n <= 74, u64 n <= 142,
 *            u128 n <= 277, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Multiplication helper that works for both fixed‑size seq_t and
   arbitrary‑precision seq_t.  For fixed‑size types (≤ 64 bits) it
   uses unsigned __int128 temporaries to avoid overflow. */
static inline seq_t binom_calc(idx_t n, idx_t k) {
    if (k > n)          /* impossible case, defensive */
        return (seq_t)0;
    if (k == 0 || k == n)
        return (seq_t)1;

    /* Use the symmetry property to reduce the number of iterations. */
    if (k > n - k)
        k = n - k;

    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        idx_t numer = n - k + i;   /* numerator in the multiplicative formula */

        /* If seq_t is a built‑in unsigned type of ≤64 bits we guard
           against overflow by performing the multiplication in
           unsigned __int128. */
        if (sizeof(seq_t) <= 8) {
            unsigned __int128 temp = (unsigned __int128)res * (unsigned __int128)numer;
            res = (seq_t)(temp / i);
        } else { /* arbitrary‑precision seq_t – rely on its operators */
            res = res * (seq_t)numer / i;
        }
    }
    return res;
}

seq_t a(idx_t n) {
    /* Compute the first k where 4*n-8*k <= k -> k >= ceil(4*n/9). */
    unsigned __int128 tmp = (unsigned __int128)n * (unsigned __int128)4 + (unsigned __int128)8;
    idx_t first_k = (idx_t)(tmp / (unsigned __int128)9);   /* ceil(4*n/9) */
    idx_t last_k  = n / (idx_t)2;                          /* floor(n/2) */

    seq_t sum = (seq_t)0;
    if (first_k > last_k)
        return sum;

    for (idx_t k = first_k; k <= last_k; ++k) {
        idx_t m = (unsigned __int128)4 * n - (unsigned __int128)8 * k;
        /* It is guaranteed that 0 <= m <= k for k in [first_k, last_k] */
        sum += binom_calc(k, m);
    }
    return sum;
}
