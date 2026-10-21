/*
 * A297116  Odd bisection of A297115, Möbius transform of A000120 (binary
 *          weight of n).
 * https://oeis.org/A297116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A297115(2n-1) = Σ_{d|(2n-1)} μ(d) * popcount((2n-1)/d)
 * Range:     i8 n <= 8944, i16 n <= 8944, i32 n <= 8944, i64 n <= 8944,
 *            i128 n <= 8944, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A297115 is the Möbius transform of the binary weight function. */

#include <stdint.h>
#include <stddef.h>

/* popcount of an unsigned 64‑bit integer */
static inline int popcount_u64(idx_t x)
{
    return __builtin_popcountll((unsigned long long)x);
}

/* Recursive enumeration of square‑free divisors of m.
   Parameters:
   primes   – array of distinct prime factors of m
   primeCnt – number of distinct primes
   pos      – current position in primes array
   cur      – product of the selected primes so far (a divisor)
   mu       – μ(cur), i.e. (-1)^{#selected primes}
   m        – the odd integer 2n-1
   sum      – accumulator for the final value */
static void dfs(const idx_t *primes, size_t primeCnt,
                size_t pos, idx_t cur, int mu,
                idx_t m, seq_t *sum)
{
    if (pos == primeCnt) {
        /* add μ(cur) * popcount(m/cur) to the sum */
        seq_t add = (seq_t)(mu * popcount_u64(m / cur));
        *sum += add;
        return;
    }

    /* exclude primes[pos] from the divisor */
    dfs(primes, primeCnt, pos + 1, cur, mu, m, sum);

    /* include primes[pos] – flip the sign of μ */
    dfs(primes, primeCnt, pos + 1, cur * primes[pos],
        -mu, m, sum);
}

/* Public function required by the OEIS harness */
seq_t a(idx_t n)
{
    /* m = 2n-1 is guaranteed to be odd */
    idx_t m = 2 * n - 1;

    /* factor m and collect distinct prime factors */
    idx_t temp = m;
    idx_t primes[64];
    size_t primeCnt = 0;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            primes[primeCnt++] = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) {
        primes[primeCnt++] = temp;
    }

    seq_t sum = 0;
    dfs(primes, primeCnt, 0, (idx_t)1, 1, m, &sum);
    return sum;
}
