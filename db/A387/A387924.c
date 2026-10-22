/*
 * A387924  Number of k <= n such that both rad(k) | n and rad(k)^2 | k, where
 *          rad = A007947.
 * https://oeis.org/A387924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers k ≤ n such that rad(k) divides n and
 *            rad(k)^2 divides k (i.e. k is powerful and its radical divides
 *            n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Factor m into distinct primes and their exponents.
   Stores primes in primes[], exponents in exps[], and returns the count. */
static size_t factor(uint64_t m, uint64_t *primes, int *exps)
{
    size_t cnt = 0;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (m > 1) {
        primes[cnt] = m;
        exps[cnt] = 1;
        ++cnt;
    }
    return cnt;
}

/* OEIS sequence A387924 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety for index 0 (off‑by‑one) */
    seq_t count = 0;
    static uint64_t primes[20];
    static int exps[20];

    for (idx_t k = 1; k <= n; ++k) {
        size_t cnt = factor((uint64_t)k, primes, exps);

        /* Check that every exponent is at least 2 (powerful). */
        bool powerful = true;
        idx_t rad = 1;
        for (size_t i = 0; i < cnt; ++i) {
            if (exps[i] < 2) {
                powerful = false;
                break;
            }
            rad *= primes[i];
        }
        if (!powerful) continue;

        /* rad divides n? */
        if (n % rad != 0) continue;

        ++count;
    }
    return count;
}
