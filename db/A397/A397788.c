/*
 * A397788  Number of odd primitive Dirichlet characters modulo n.
 * https://oeis.org/A397788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of odd primitive Dirichlet characters modulo n.
 *            Computation uses the known formulas for the total number of
 *            primitive characters b(n) (sequence A007431) and the parity rule
 *            described in the OEIS entry: - For a prime power p^e (p odd):
 *            b(p) = p - 2 a(p) = (p - 1) / 2 b(p^e) = (p - 1)...
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)0;       /* a(1)=0 */

    seq_t b_total = (seq_t)1;          /* b(1)=1 */
    unsigned omega = 0;                /* distinct prime factors */

    idx_t t = n;

    /* handle factor 2 */
    if (t % 2 == 0) {
        unsigned e = 0;
        while (t % 2 == 0) { t /= 2; ++e; }

        seq_t local_b;
        if (e == 1) {
            local_b = (seq_t)0;
        } else if (e == 2) {
            local_b = (seq_t)1;
        } else {
            local_b = ((seq_t)1) << (e - 2);   /* 2^(e-2) */
        }
        b_total *= local_b;
        omega += 1;
    }

    /* other odd primes */
    for (idx_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            unsigned e = 0;
            while (t % p == 0) { t /= p; ++e; }

            seq_t local_b;
            if (e == 1) {
                local_b = (seq_t)(p - 2);
            } else {
                seq_t p_pow = (seq_t)1;
                for (unsigned i = 0; i < e - 2; ++i) p_pow *= (seq_t)p;
                local_b = ((seq_t)(p - 1)) * ((seq_t)(p - 1)) * p_pow;
            }
            b_total *= local_b;
            omega += 1;
        }
    }

    /* remaining prime factor > sqrt(t) */
    if (t > 1) {
        seq_t local_b = (seq_t)(t - 2);
        b_total *= local_b;
        omega += 1;
    }

    /* compute a(n) from b(n) and parity rule */
    if ((b_total & 1) == 0)  /* b even */
        return b_total / 2;
    else {                   /* b odd */
        int sign = (omega & 1) ? -1 : 1;   /* (-1)^omega */
        return (b_total - (seq_t)sign) / 2;
    }
}
