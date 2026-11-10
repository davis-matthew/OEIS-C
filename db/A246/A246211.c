/*
 * A246211  Self-inverse permutation of natural numbers: a(0) = 0, a(1) = 1,
 *          and for n > 1, if A117966(n) < 0, a(n) =
 *          A117967(1+a(-(A117966(n)))), otherwise a(n) =
 *          A117968(a(A117966(n)-1)).
 * https://oeis.org/A246211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1; for n>1, with v=A117966(n): if v<0,
 *            a(n)=A117967(1+a(-v)), else a(n)=A117968(a(v-1))
 * Range:     u8 n <= 9, u16 n <= 41, u32 n <= 284, u64 n <= 2187,
 *            u128 n <= 2187, big any n
 * Validated: standard, pass, 90 terms through n = 2173 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A117967_f(seq_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t r = n % (seq_t)3;
    if (r == 0) return A117967_f(n / (seq_t)3) * (seq_t)3;
    if (r == 1) return A117967_f(n / (seq_t)3) * (seq_t)3 + (seq_t)1;
    return A117967_f((n + (seq_t)1) / (seq_t)3) * (seq_t)3 + (seq_t)2;
}

static seq_t A117968_f(seq_t n)
{
    if (n == 1) return (seq_t)2;
    seq_t r = n % (seq_t)3;
    if (r == 0) return (seq_t)3 * A117968_f(n / (seq_t)3);
    if (r == 1) return (seq_t)3 * A117968_f((n - (seq_t)1) / (seq_t)3) + (seq_t)2;
    return (seq_t)3 * A117968_f((n + (seq_t)1) / (seq_t)3) + (seq_t)1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    /* v = A117966(n), signed balanced ternary value */
    __int128 v = 0, p = 1;
    idx_t t = n;
    while (t > 0) {
        int d = (int)(t % 3);
        if (d == 1) v += p;
        else if (d == 2) v -= p;
        p *= 3;
        t /= 3;
    }
    if (v < 0) {
        idx_t m = (idx_t)(-v);
        seq_t x = a(m);
        return A117967_f(x + (seq_t)1);
    } else {
        idx_t m = (idx_t)(v - 1);
        seq_t x = a(m);
        return A117968_f(x);
    }
}
