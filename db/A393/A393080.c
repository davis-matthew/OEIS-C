/*
 * A393080  Second column of A392806.
 * https://oeis.org/A393080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(3^(e+1)*m + 3^e*k) = a(3^(e+1)*m) + k*max(0, 2*3^e*S(m) -
 *            A392084(e)), S(m)=Sum_{i=1..m} A051064(i), e=v_3(n), k in {1,2};
 *            a(0)=0
 * Range:     u8 n <= 79, u16 n <= 6559, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9162 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* sum_{i=1..m} (v3(i)+1) = m + floor(m/3) + floor(m/9) + ... */
static seq_t A393080_S(idx_t m)
{
    seq_t s = (seq_t)m;
    idx_t q = m / 3;
    while (q > 0) {
        s += (seq_t)q;
        q /= 3;
    }
    return s;
}

/* A392084(e) = 3*A(e-1) + 2*3^e - 1, A(0)=0 */
static seq_t A393080_A392084(idx_t e)
{
    seq_t A = 0;
    seq_t pw = 1;
    for (idx_t i = 1; i <= e; ++i) {
        pw *= (seq_t)3;
        A = (seq_t)3 * A + (seq_t)2 * pw - (seq_t)1;
    }
    return A;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    while (n > 0) {
        idx_t e = 0;
        idx_t p = 1;
        idx_t t = n;
        while (t % 3 == 0) {
            t /= 3;
            p *= 3;
            e++;
        }
        idx_t k = t % 3;
        idx_t m = t / 3;
        seq_t T = (seq_t)2 * (seq_t)p * A393080_S(m);
        seq_t A = A393080_A392084(e);
        if (T > A)
            total += (seq_t)k * (T - A);
        n = m * p * 3;
    }
    return total;
}
