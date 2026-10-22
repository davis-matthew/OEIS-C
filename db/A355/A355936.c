/*
 * A355936  Dirichlet inverse of A295316, characteristic function of
 *          exponentially odd numbers.
 * https://oeis.org/A355936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = ∏_{p^e‖n} (-1)^e * Fib(e), where Fib(0)=0, Fib(1)=1,
 *            Fib(k)=Fib(k-1)+Fib(k-2)
 * Range:     i8 n <= 4095, i16 n <= 8920, i32 n <= 8920, i64 n <= 8920,
 *            i128 n <= 8920, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute the e‑th Fibonacci number (e ≥ 0). */
static seq_t fib_of(uint64_t e)
{
    if (e == 0) return 0;
    if (e == 1) return 1;
    seq_t prev = 0;
    seq_t curr = 1;
    for (uint64_t i = 2; i <= e; ++i) {
        seq_t tmp = curr + prev;
        prev = curr;
        curr = tmp;
    }
    return curr;
}

/* Dirichlet inverse of A295316: characteristic function of
   exponentially odd numbers.  Sequence is multiplicative with
   a(p^e) = (-1)^e * Fib(e). */
seq_t a(idx_t n)
{
    if (n == 1) return 1;
    seq_t result = 1;
    uint64_t m = n;
    while (m > 1) {
        /* find smallest prime divisor of m */
        uint64_t p = 0;
        if ((m & 1) == 0) {
            p = 2;
        } else {
            for (uint64_t d = 3; d * d <= m; d += 2) {
                if (m % d == 0) { p = d; break; }
            }
        }
        if (p == 0) p = m;  /* m itself is prime */

        /* count exponent of p in m */
        uint64_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }

        /* compute contribution (-1)^e * Fib(e) */
        seq_t term = fib_of(e);
        if (e & 1) term = -term;
        result *= term;
    }
    return result;
}
