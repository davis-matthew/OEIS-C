/*
 * A374121  a(n) = 1 if A328845(n) is a multiple of 3, otherwise 0, where
 *          A328845 is the first Fibonacci-based variant of the arithmetic
 *          derivative.
 * https://oeis.org/A374121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A328845(n) is a multiple of 3, otherwise 0.
 *            A328845(n) is the Fibonacci‑based arithmetic derivative, defined
 *            by d(1) = 0, d(p) = F_p (Fibonacci number), and for m,n>1 d(mn)
 *            = m·d(n) + n·d(m). For a prime power p^e d(p^e) = e·p^{e-1}·F_p.
 *            The derivative of a composite is obtained from...
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const int fib_mod3[8] = {0,1,1,2,0,2,2,1};

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;                     /* d(0)=d(1)=0 is divisible by 3 */

    idx_t m = n;
    int sum_mod3 = 0;

    /* factor the even part */
    if ((m & 1) == 0) {
        int exp = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            ++exp;
        }
        int e_mod3 = exp % 3;
        int w_mod3 = fib_mod3[(int)(2 % 8)];   /* F_2 = 1, mod3=1 */
        int n_div_mod3 = (n / 2) % 3;
        int term = (e_mod3 * w_mod3) % 3;
        term = (term * n_div_mod3) % 3;
        sum_mod3 = (sum_mod3 + term) % 3;
    }

    /* factor odd primes */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            int e_mod3 = exp % 3;
            int w_mod3 = fib_mod3[(int)(p % 8)];
            int n_div_mod3 = (n / p) % 3;
            int term = (e_mod3 * w_mod3) % 3;
            term = (term * n_div_mod3) % 3;
            sum_mod3 = (sum_mod3 + term) % 3;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        int exp = 1;
        int e_mod3 = exp % 3;           /* 1 */
        int w_mod3 = fib_mod3[(int)(m % 8)];
        int n_div_mod3 = (n / m) % 3;
        int term = (e_mod3 * w_mod3) % 3;
        term = (term * n_div_mod3) % 3;
        sum_mod3 = (sum_mod3 + term) % 3;
    }

    return (sum_mod3 == 0) ? 1 : 0;
}
