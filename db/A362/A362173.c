/*
 * A362173  a(n) = n! * Sum_{k=0..floor(n/3)} (n/6)^k *
 *          binomial(n-2*k,k)/(n-2*k)!.
 * https://oeis.org/A362173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} (n/6)^k *
 *            binomial(n-2*k,k)/(n-2*k)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


static inline seq_t _seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    seq_t term = (seq_t)1;          /* t_0 */
    sum += term;

    idx_t max_k = n / 3;
    for (idx_t k = 1; k <= max_k; ++k) {
        seq_t denom[2];
        denom[0] = (seq_t)6;
        denom[1] = (seq_t)k;

        /* factor 1 : n */
        seq_t f = (seq_t)n;
        term *= f;
        for (int i = 0; i < 2; ++i) {
            seq_t g = _seq_gcd(term, denom[i]);
            if (g > (seq_t)1) { term /= g; denom[i] /= g; }
        }

        /* factor 2 : n - 3*k + 3 */
        f = (seq_t)(n - 3*k + 3);
        term *= f;
        for (int i = 0; i < 2; ++i) {
            seq_t g = _seq_gcd(term, denom[i]);
            if (g > (seq_t)1) { term /= g; denom[i] /= g; }
        }

        /* factor 3 : n - 3*k + 2 */
        f = (seq_t)(n - 3*k + 2);
        term *= f;
        for (int i = 0; i < 2; ++i) {
            seq_t g = _seq_gcd(term, denom[i]);
            if (g > (seq_t)1) { term /= g; denom[i] /= g; }
        }

        /* factor 4 : n - 3*k + 1 */
        f = (seq_t)(n - 3*k + 1);
        term *= f;
        for (int i = 0; i < 2; ++i) {
            seq_t g = _seq_gcd(term, denom[i]);
            if (g > (seq_t)1) { term /= g; denom[i] /= g; }
        }

        /* divide any remaining denominators */
        if (denom[0] > (seq_t)1) term /= denom[0];
        if (denom[1] > (seq_t)1) term /= denom[1];

        sum += term;
    }
    return sum;
}
