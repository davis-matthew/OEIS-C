/*
 * A363888  Number of divisors of 7*n-5 of form 7*k+4.
 * https://oeis.org/A363888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of (7*n-5) with d ≡ 4 (mod 7)
 * Range:     u8 n <= 38, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>            /* for size_t */

static void count_divisors(const uint64_t primes[], const uint64_t exps[],
                           size_t cnt, size_t idx, seq_t cur, seq_t *cnt_res)
{
    if (idx == cnt) {
        if (cur % 7 == 4)
            (*cnt_res)++;
        return;
    }
    seq_t prod = cur;
    for (uint64_t e = 0; e <= exps[idx]; e++) {
        count_divisors(primes, exps, cnt, idx + 1, prod, cnt_res);
        prod = prod * (seq_t)primes[idx];
    }
}
 
seq_t a(idx_t n)
{
    /* Compute the argument m = 7*n-5 */
    seq_t m = 7 * (seq_t)n - 5;
    /* The value fits into 64 bits for the intended range (n≈10 000) */
    uint64_t m64 = (uint64_t)m;

    /* Prime factorisation of m – trial division suffices for the size. */
    uint64_t primes[16], exps[16];
    size_t pcnt = 0;
    uint64_t tmp = m64;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            pcnt++;
        }
    }
    if (tmp > 1) {               /* remaining prime factor */
        primes[pcnt] = tmp;
        exps[pcnt]   = 1;
        pcnt++;
    }

    /* Enumerate all divisors and count those ≡4 (mod 7). */
    seq_t result = 0;
    count_divisors(primes, exps, pcnt, 0, (seq_t)1, &result);
    return result;
}
