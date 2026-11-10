/*
 * A360123  Parity of A347389, where A347389 is Dirichlet convolution of
 *          A003415(n) with A003415(A276086(n)).
 * https://oeis.org/A360123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A347389(n) mod 2, where A347389(n) = Sum_{d|n}
 *            A003415(n/d)*A003415(A276086(d)); computed mod 2 directly
 * Range:     u8 n <= 30030, u16 n <= 30030, u32 n <= 30030, u64 n <= 30030,
 *            u128 n <= 30030, big any n
 * Validated: standard, pass, 137 terms through n = 7503 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A360123_deriv_par(uint64_t m)
{
    if (m <= 1) return 0;
    uint64_t sum = 0, tmp = m;
    for (uint64_t p = 2; p * p <= tmp; p++) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) { tmp /= p; e++; }
            sum += e * (m / p);
        }
    }
    if (tmp > 1) sum += m / tmp;
    return sum & 1u;
}

static uint64_t A360123_next_prime(uint64_t p)
{
    for (uint64_t c = p + 1;; c++) {
        bool ok = true;
        for (uint64_t q = 2; q * q <= c; q++)
            if (c % q == 0) { ok = false; break; }
        if (ok) return c;
    }
}

/* parity of A003415(A276086(d)) */
static uint64_t A360123_par_A327860(uint64_t d)
{
    if (d & 1u) return 1;          /* exponent of 2 is 1 -> derivative odd */
    uint64_t q = d / 2, p = 3, s = 0;
    while (q > 0) {
        s += q % p;
        q /= p;
        p = A360123_next_prime(p);
    }
    return s & 1u;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n, total = 0;
    for (uint64_t i = 1; i * i <= N; ++i) {
        if (N % i != 0) continue;
        uint64_t d1 = i, d2 = N / i;
        total ^= A360123_deriv_par(N / d1) & A360123_par_A327860(d1);
        if (d1 != d2)
            total ^= A360123_deriv_par(N / d2) & A360123_par_A327860(d2);
    }
    return (seq_t)(total & 1u);
}
